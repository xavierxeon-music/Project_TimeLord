#include "MainWidget.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QJsonArray>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>

#include <AppSettings.h>
#include <FileSettings.h>

#include "RampDeviceVCV.h"
#include "RampModel.h"
#include "TempoWidget.h"

MainWidget::MainWidget()
   : QWidget(nullptr)
   , Data::Core()
   , splitter(nullptr)
   , statusBar(nullptr)
   , polyRampVisu(nullptr)
   , polyRampWidget(nullptr)
   , stageWidget(nullptr)
   , polyLineWidget(nullptr)
{
   setWindowTitle("Time Lord UI[*]");
   setMinimumSize(1400, 900);

   RampDevice::VCV* device = new RampDevice::VCV(this);

   createRampDevice(this);

   statusBar = new QStatusBar(this);
   statusBar->setSizeGripEnabled(true);

   TempoWidget* tempoWidget = new TempoWidget(this, raspiDevice->getTempo());
   statusBar->addPermanentWidget(tempoWidget);

   loadLastFile();

   polyRampVisu = new Ramp::Visu(this);
   polyRampWidget = new Ramp::Widget(this);
   stageWidget = new Stage::Widget(this);
   polyLineWidget = new PolyLine::Widget(this);

   splitter = new QSplitter(Qt::Horizontal, this);
   splitter->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   splitter->setObjectName("ModelSplitter");
   splitter->addWidget(polyRampWidget);
   splitter->addWidget(stageWidget);
   splitter->addWidget(polyLineWidget);


   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(polyRampVisu);
   masterLayout->addWidget(splitter);
   masterLayout->addWidget(statusBar);

   AppSettings widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   splitter->restoreState(widgetSettings.bytes("StateModel"));

   QTimer* modfifiedCheckTimer = new QTimer(this);
   connect(modfifiedCheckTimer, &QTimer::timeout, this, &MainWidget::slotCheckDataModified);
   modfifiedCheckTimer->start(1000);
}

void MainWidget::forceRebuildModels()
{
   Data::Identifier dummy;
   callOnAllInstances(&Core::rebuildModel, dummy);
}

void MainWidget::slotLoadFromFile()
{
   const QString fileName = QFileDialog::getOpenFileName(this, "Save Data", QString(), "*.timelord.json");
   if (fileName.isEmpty())
      return;

   AppSettings appSettings;
   appSettings.write("LastFile", fileName);

   FileSettings::setFileName(fileName);
   loadInternal(fileName);
}

void MainWidget::slotSaveToFile()
{
   AppSettings appSettings;
   QString fileName = appSettings.string("LastFile");

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

   AppSettings appSettings;
   appSettings.write("LastFile", fileName);

   FileSettings::setFileName(fileName);
   saveInternal(fileName);
}

void MainWidget::slotSaveToRaspi()
{
   raspiDevice->pushToServer();
}

void MainWidget::slotEnableMidiOutput(bool enabled)
{
   raspiDevice->enableMidiPort(enabled);
}

void MainWidget::slotCheckDataModified()
{
   if (raspiDevice->isUnsynced() || isModified)
      setWindowModified(true);
   else
      setWindowModified(false);
}

void MainWidget::loadLastFile()
{
   AppSettings appSettings;
   QString fileName = appSettings.string("LastFile");
   if (fileName.isEmpty())
      return;

   if (!QFile::exists(fileName))
   {
      appSettings.write("LastFile", QString());
      return;
   }

   FileSettings::setFileName(fileName);
   loadInternal(fileName);
}

void MainWidget::loadInternal(const QString& fileName)
{
   FileSettings settings;
   const QByteArray content = settings.bytes("binary");

   RootStorage storage(raspiDevice);
   storage.loadFromData(content);

   updateWindowTitle(fileName);
   forceRebuildModels();

   statusBar->showMessage("file loaded", 2000);
}

void MainWidget::saveInternal(const QString& fileName)
{
   callOnAllInstances(&Core::saveSettings);
   unsetModified();

   RootStorage storage(raspiDevice);
   const QByteArray content = storage.saveToData();
   raspiDevice->setSynced();

   QJsonObject ramps;
   for (uint8_t rampIndex = 0; rampIndex < 16; rampIndex++)
   {
      Data::Identifier identifier(rampIndex);
      PolyRamp* polyRamp = getPolyRamp(identifier);

      QJsonArray stageArray;
      for (uint8_t stageIndex = 0; stageIndex < polyRamp->stageCount(); stageIndex++)
      {
         QJsonObject stageObject;
         stageObject["length"] = polyRamp->getStageLength(stageIndex);
         stageObject["startHeight"] = polyRamp->getStageStartHeight(stageIndex);
         stageArray.append(stageObject);
      }

      QJsonObject rampObject;
      rampObject["stages"] = stageArray;
      rampObject["stepSize"] = polyRamp->getStepSize();
      rampObject["length"] = static_cast<int64_t>(polyRamp->getLength());
      rampObject["lopp"] = polyRamp->isLooping();
      ramps[keys.at(rampIndex)] = rampObject;
   }

   FileSettings settings;
   settings.write("binary", content);
   settings.write("ramps", ramps);

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
