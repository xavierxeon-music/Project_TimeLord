#include "MainWidget.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>

#include <SettingsUI.h>

#include "GraphAudioDevice.h"

using Channel = AudioDevice::Channel;
using Frame = AudioDevice::Frame;

MainWidget::MainWidget()
   : QWidget(nullptr)
   , Remember::Root()
   , graphs(this)
   , graphAudioDevice(new GraphAudioDevice(this))
   , midiBridge(this)
   , fileStorageDaisy(this)
   , fileStorageDevice(graphAudioDevice)
   , splitter(nullptr)
   , graphWidget(nullptr)
   , graphModel(nullptr)
   , stageWidget(nullptr)
   , stageModel(nullptr)
   , graphVisuWidget(nullptr)
{
   setWindowTitle("Time Lord UI[*]");

   midiBridge.initMidi();

   QToolBar* toolBar = new QToolBar(this);
   toolBar->setIconSize(QSize(24, 24));

   graphModel = new GraphModel(this);
   stageModel = new StageModel(this);

   graphWidget = new GraphWidget(this, toolBar, graphModel);
   stageWidget = new StageWidget(this, toolBar, stageModel);
   graphVisuWidget = new GraphVisuWidget(this, toolBar);

   connect(graphWidget, &GraphWidget::signalPortChanged, stageModel, &StageModel::slotPortChanged);
   graphModel->update();

   splitter = new QSplitter(this);
   splitter->setObjectName("MainSplitter");
   splitter->addWidget(graphWidget);
   splitter->addWidget(stageWidget);
   splitter->addWidget(graphVisuWidget);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(splitter);

   SettingsUI widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   splitter->restoreState(widgetSettings.bytes("State"));

   loadLastFile();

   QTimer* modfifiedCheckTimer = new QTimer(this);
   connect(modfifiedCheckTimer, &QTimer::timeout, this, &MainWidget::slotCheckDataModified);
   modfifiedCheckTimer->start(1000);
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
   if (isUnsynced() || graphAudioDevice->isUnsynced())
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
   updateUI();
}

void MainWidget::saveInternal(const QString& fileName)
{
   fileStorageDaisy.saveToFile(fileName);
   fileStorageDevice.saveToFile(fileName + ".device");

   updateWindowTitle(fileName);
}

void MainWidget::updateWindowTitle(const QString& fileName)
{
   QFileInfo fileInfo(fileName);
   const QString documentName = fileInfo.baseName();

   setWindowTitle("Time Lord UI - " + documentName + "[*]");
}

void MainWidget::updateUI()
{
   graphModel->update();
   stageModel->update();
}

void MainWidget::closeEvent(QCloseEvent* ce)
{
   if (isWindowModified())
   {
   }

   SettingsUI widgetSettings("MainWidget");
   widgetSettings.write("Geometry", saveGeometry());
   widgetSettings.write("State", splitter->saveState());

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
