#include "MainWidget.h"

#include <QApplication>
#include <QCloseEvent>
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
   , deviceWidget(nullptr)
   , deviceModel(nullptr)
   , pointWidget(nullptr)
   , pointModel(nullptr)
   , graphWidget(nullptr)
{
   setWindowTitle("Time Lord UI");

   midiBridge.initMidi();
   midiBridge.onLoadedFromDaisy(this, &MainWidget::loadedFromDaisy);

   QToolBar* toolBar = new QToolBar(this);
   toolBar->setIconSize(QSize(24, 24));

   deviceModel = new DeviceModel(this);
   pointModel = new PointModel(this);

   deviceWidget = new DeviceWidget(this, toolBar, deviceModel);
   pointWidget = new PointWidget(this, toolBar, pointModel);
   graphWidget = new GraphWidget(this, toolBar);

   connect(deviceWidget, &DeviceWidget::signalPortChanged, pointModel, &PointModel::slotPortChanged);
   deviceModel->init();

   splitter = new QSplitter(this);
   splitter->setObjectName("MainSplitter");
   splitter->addWidget(deviceWidget);
   splitter->addWidget(pointWidget);
   splitter->addWidget(graphWidget);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(splitter);

   SettingsUI widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   splitter->restoreState(widgetSettings.bytes("State"));
}

void MainWidget::slotNewFile()
{
   const QString fileName;
   fileStorageDaisy.loadFromFile(fileName);
   fileStorageDevice.loadFromFile(fileName);
}

void MainWidget::slotLoadFromFile()
{
   const QString fileName;
   fileStorageDaisy.loadFromFile(fileName);
   fileStorageDevice.loadFromFile(fileName);
   updateUI();
}

void MainWidget::slotSaveToFile()
{
   const QString fileName;
   fileStorageDaisy.saveToFile(fileName);
   fileStorageDevice.saveToFile(fileName);
}

void MainWidget::slotSaveToDaisy()
{
   midiBridge.saveToDaisy();
}

void MainWidget::loadedFromDaisy()
{
   updateUI();
}

void MainWidget::updateUI()
{
}

void MainWidget::closeEvent(QCloseEvent* ce)
{
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
