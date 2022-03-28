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
   , graphs(this)
   , audioDevice(new AudioDeviceGraph(this))
   , midiBridge(this)
   , fileStorageDaisy(this)
   , fileStorageDevice(audioDevice)
   , splitterModel(nullptr)
   , splitterVisu(nullptr)
   , statusBar(nullptr)
   , graphWidget(nullptr)
   , graphModel(nullptr)
   , stageWidget(nullptr)
   , stageModel(nullptr)
   , graphVisuWidget(nullptr)
{
   setWindowTitle("Time Lord UI[*]");

   midiBridge.initMidi();

   graphModel = new GraphModel(this);
   stageModel = new StageModel(this);

   graphWidget = new GraphWidget(this, graphModel);
   stageWidget = new StageWidget(this, stageModel);
   graphVisuWidget = new GraphVisuWidget(this);

   connect(graphWidget, &GraphWidget::signalGraphSelected, stageWidget, &StageWidget::slotGraphSelected);
   connect(graphWidget, &GraphWidget::signalGraphSelected, graphVisuWidget, &GraphVisuWidget::slotGraphSelected);
   connect(stageModel, &StageModel::signalGraphLengthChanged, graphModel, &GraphModel::slotGraphLengthChanged);
   graphModel->rebuild();

   splitterModel = new QSplitter(Qt::Horizontal, this);
   splitterModel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   splitterModel->setObjectName("ModelSplitter");
   splitterModel->addWidget(graphWidget);
   splitterModel->addWidget(stageWidget);

   splitterVisu = new QSplitter(Qt::Vertical, this);
   splitterModel->setObjectName("VisuSplitter");
   splitterVisu->addWidget(splitterModel);
   splitterVisu->addWidget(graphVisuWidget);

   statusBar = new QStatusBar(this);
   statusBar->setSizeGripEnabled(true);

   TempoWidget* tempoWidget = new TempoWidget(this, audioDevice->getTempo());
   statusBar->addPermanentWidget(tempoWidget);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(splitterVisu);
   masterLayout->addWidget(statusBar);

   SettingsUI widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   splitterModel->restoreState(widgetSettings.bytes("StateModel"));
   splitterVisu->restoreState(widgetSettings.bytes("StateVisu"));

   loadLastFile();

   QTimer* modfifiedCheckTimer = new QTimer(this);
   connect(modfifiedCheckTimer, &QTimer::timeout, this, &MainWidget::slotCheckDataModified);
   modfifiedCheckTimer->start(1000);

   for (uint8_t index = 0; index < 16; index++)
      graphs[index].clockReset();
   audioDevice->clockReset();
}

void MainWidget::forceRebuildModels()
{
   graphModel->rebuild();
   stageModel->rebuild(Model::Provider::None, 0, false);
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
   widgetSettings.write("StateModel", splitterModel->saveState());
   widgetSettings.write("StateVisu", splitterVisu->saveState());

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
