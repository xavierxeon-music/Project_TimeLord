#include "MainWidget.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>

#include <SettingsUI.h>

#include "AudioDeviceGraph.h"
#include "TempoWidget.h"

using Channel = AudioDevice::Channel;
using Frame = AudioDevice::Frame;

MainWidget::MainWidget()
   : QWidget(nullptr)
   , Remember::Root()
   , polyRamps(this)
   , audioDevice(new AudioDeviceGraph(this))
   , midiBridge(this)
   , fileStorageDaisy(this)
   , fileStorageDevice(audioDevice)
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

   midiBridge.initMidi();

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

   TempoWidget* tempoWidget = new TempoWidget(this, audioDevice->getTempo());
   statusBar->addPermanentWidget(tempoWidget);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(polyRampVisu);
   masterLayout->addWidget(splitter);
   masterLayout->addWidget(statusBar);

   SettingsUI widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   splitter->restoreState(widgetSettings.bytes("StateModel"));

   loadLastFile();

   QTimer* modfifiedCheckTimer = new QTimer(this);
   connect(modfifiedCheckTimer, &QTimer::timeout, this, &MainWidget::slotCheckDataModified);
   modfifiedCheckTimer->start(1000);

   for (uint8_t index = 0; index < 16; index++)
      polyRamps[index].clockReset();
   audioDevice->clockReset();
}

void MainWidget::forceRebuildModels()
{
   polyRampModel->rebuild();
   stageModel->rebuild(Data::Identifier());
}

void MainWidget::slotLoadFromFile()
{
   const QString fileName = QFileDialog::getOpenFileName(this, "Save Data", QString(), "*.timelord");
   if (fileName.isEmpty())
      return;

   SettingsUI fileSettings;
   fileSettings.write("LastFile", fileName);

   loadInternal(fileName);
}

void MainWidget::slotSaveToFile()
{
   SettingsUI fileSettings;
   QString fileName = fileSettings.string("LastFile");

   if (fileName.isEmpty())
      return slotSaveNewFile();

   saveInternal(fileName);
}

void MainWidget::slotSaveNewFile()
{
   QString fileName = QFileDialog::getSaveFileName(this, "Save Data", QString(), "*.timelord");
   if (fileName.isEmpty())
      return;

   if (!fileName.endsWith(".timelord"))
      fileName += ".timelord";

   SettingsUI fileSettings;
   fileSettings.write("LastFile", fileName);

   saveInternal(fileName);
}

void MainWidget::slotSaveToDaisy()
{
   midiBridge.saveToDaisy();
}

void MainWidget::slotCheckDataModified()
{
   if (isUnsynced() || audioDevice->isUnsynced())
      setWindowModified(true);
   else
      setWindowModified(false);
}

void MainWidget::loadLastFile()
{
   SettingsUI fileSettings;
   QString fileName = fileSettings.string("LastFile");
   if (fileName.isEmpty())
      return;

   loadInternal(fileName);
}

void MainWidget::loadInternal(const QString& fileName)
{
   fileStorageDaisy.loadFromFile(fileName);
   fileStorageDevice.loadFromFile(fileName + ".device");

   updateWindowTitle(fileName);
   forceRebuildModels();

   statusBar->showMessage("file loaded", 2000);
}

void MainWidget::saveInternal(const QString& fileName)
{
   fileStorageDaisy.saveToFile(fileName);
   fileStorageDevice.saveToFile(fileName + ".device");

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

   SettingsUI widgetSettings("MainWidget");
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
