#include "MainWidget.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>

#include <AppSettings.h>
#include <FileSettings.h>

#include "TempoWidget.h"

MainWidget::MainWidget()
   : QWidget(nullptr)
   , raspiDevice(this)
   , splitter(nullptr)
   , statusBar(nullptr)
   , polyRampModel(nullptr)
   , polyRampVisu(nullptr)
   , polyRampWidget(nullptr)
   , stageModel(nullptr)
   , stageWidget(nullptr)
   , polyLineModel(nullptr)
   , polyLineWidget(nullptr)
{
   setWindowTitle("Time Lord UI[*]");
   setMinimumSize(1400, 900);

   polyRampModel = new Ramp::Model(this);
   stageModel = new Stage::Model(this);
   polyLineModel = new PolyLine::Model(this);

   polyRampVisu = new Ramp::Visu(this);
   polyRampWidget = new Ramp::Widget(this, polyRampModel);
   stageWidget = new Stage::Widget(this, stageModel);
   polyLineWidget = new PolyLine::Widget(this, polyLineModel);

   connect(polyRampWidget, &Ramp::Widget::signalGraphSelected, polyRampVisu, &Ramp::Visu::slotGraphSelected);
   connect(stageModel, &Stage::Model::signalRampChanged, polyRampModel, &Ramp::Model::slotRampChanged);
   connect(polyRampWidget, &Ramp::Widget::signalGraphSelected, stageWidget, &Stage::Widget::slotGraphSelected);
   connect(polyRampWidget, &Ramp::Widget::signalGraphSelected, polyLineWidget, &PolyLine::Widget::slotGraphSelected);
   polyRampModel->rebuild();

   splitter = new QSplitter(Qt::Horizontal, this);
   splitter->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   splitter->setObjectName("ModelSplitter");
   splitter->addWidget(polyRampWidget);
   splitter->addWidget(stageWidget);
   splitter->addWidget(polyLineWidget);

   statusBar = new QStatusBar(this);
   statusBar->setSizeGripEnabled(true);

   TempoWidget* tempoWidget = new TempoWidget(this, raspiDevice.getTempo());
   statusBar->addPermanentWidget(tempoWidget);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(polyRampVisu);
   masterLayout->addWidget(splitter);
   masterLayout->addWidget(statusBar);

   AppSettings widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   splitter->restoreState(widgetSettings.bytes("StateModel"));

   loadLastFile();

   QTimer* modfifiedCheckTimer = new QTimer(this);
   connect(modfifiedCheckTimer, &QTimer::timeout, this, &MainWidget::slotCheckDataModified);
   modfifiedCheckTimer->start(1000);
}

void MainWidget::forceRebuildModels()
{
   polyRampModel->rebuild();
   stageModel->rebuild(Data::Identifier());
}

void MainWidget::slotLoadFromFile()
{
   const QString fileName = QFileDialog::getOpenFileName(this, "Save Data", QString(), "*.timelord.json");
   if (fileName.isEmpty())
      return;

   AppSettings fileSettings;
   fileSettings.write("LastFile", fileName);

   FileSettings::setFileName(fileName);
   loadInternal(fileName);
}

void MainWidget::slotSaveToFile()
{
   AppSettings fileSettings;
   QString fileName = fileSettings.string("LastFile");

   if (fileName.isEmpty())
      return slotSaveNewFile();

   saveInternal(fileName);
}

void MainWidget::slotSaveNewFile()
{
   QString fileName = QFileDialog::getSaveFileName(this, "Save Data", QString(), "*.json");
   if (fileName.isEmpty())
      return;

   if (!fileName.endsWith(".timelord.json"))
   {
      if (!fileName.endsWith(".json"))
         fileName += ".timelord.json";
      else
         fileName.replace(".json", ".timelord.json");
   }

   AppSettings fileSettings;
   fileSettings.write("LastFile", fileName);

   FileSettings::setFileName(fileName);
   saveInternal(fileName);
}

void MainWidget::slotSaveToRaspi()
{
   raspiDevice.pushToServer();
}

void MainWidget::slotEnableMidiOutput(bool enabled)
{
   raspiDevice.enableMidiPort(enabled);
}

void MainWidget::slotCheckDataModified()
{
   if (raspiDevice.isUnsynced())
      setWindowModified(true);
   else
      setWindowModified(false);
}

void MainWidget::loadLastFile()
{
   AppSettings fileSettings;
   QString fileName = fileSettings.string("LastFile");
   if (fileName.isEmpty())
      return;

   if (!QFile::exists(fileName))
      return;

   FileSettings::setFileName(fileName);
   loadInternal(fileName);
}

void MainWidget::loadInternal(const QString& fileName)
{
   FileStorage fileStorageRaspi(&raspiDevice);
   fileStorageRaspi.loadFromFile(QString(fileName).replace(".json", ".raspi"));

   updateWindowTitle(fileName);
   forceRebuildModels();

   statusBar->showMessage("file loaded", 2000);
}

void MainWidget::saveInternal(const QString& fileName)
{
   FileStorage fileStorageRaspi(&raspiDevice);
   fileStorageRaspi.saveToFile(QString(fileName).replace(".json", ".raspi"));

   updateWindowTitle(fileName);

   statusBar->showMessage("file saved", 2000);
}

void MainWidget::updateWindowTitle(const QString& fileName)
{
   QFileInfo fileInfo(fileName);
   const QString documentName = fileInfo.baseName();

   setWindowTitle("Time Lord UI - " + documentName + "[*]");
}

void MainWidget::closeEvent(QCloseEvent* ce)
{
   if (isWindowModified())
   {
   }

   AppSettings widgetSettings("MainWidget");
   widgetSettings.write("Geometry", saveGeometry());
   widgetSettings.write("StateModel", splitter->saveState());

   ce->accept();
}

// main function

int main(int argc, char** argv)
{
   QApplication::setApplicationName("TimeLordUI");
   QApplication::setOrganizationDomain("eu.schweinesystem");

   QApplication app(argc, argv);

   MainWidget mw;
   mw.show();

   return app.exec();
}
