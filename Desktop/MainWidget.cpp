#include "MainWidget.h"

#include <QApplication>
#include <QCloseEvent>

#include <SettingsUI.h>

#include "GraphDevice.h"

using Channel = AudioDevice::Channel;
using Frame = AudioDevice::Frame;

MainWidget::MainWidget()
   : QSplitter(nullptr)
   , Remember::Root()
   , graphs(this)
   , graphDevice(new GraphDevice(this))
   , midiBridge(this)
   , fileStorageDaisy(this)
   , fileStorageDevice(graphDevice)
   , portWidget(nullptr)
   , portModel(nullptr)
   , pointWidget(nullptr)
   , pointModel(nullptr)
   , graphWidget(nullptr)
{
   setWindowTitle("Time Lord UI");

   midiBridge.initMidi();
   midiBridge.onLoadedFromDaisy(this, &MainWidget::loadedFromDaisy);


   portModel = new PortModel(this);
   pointModel = new PointModel(this);

   portWidget = new PortWidget(this, portModel);
   pointWidget = new PointWidget(this, pointModel);
   graphWidget = new GraphWidget(this);

   connect(portWidget, &PortWidget::signalPortChanged, pointModel, &PointModel::slotPortChanged);
   portModel->init();

   addWidget(portWidget);
   addWidget(pointWidget);
   addWidget(graphWidget);

   SettingsUI widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   restoreState(widgetSettings.bytes("State"));
}

void MainWidget::slotLoadFromFile()
{
   const QString fileName;
   fileStorageDaisy.loadFromFile(fileName);
   fileStorageDevice.loadFromFile(fileName);
   updateUI();
}

void MainWidget::slotStartLoadFromdDaisy()
{
   midiBridge.requestLoadFromDaisy();
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
   widgetSettings.write("State", saveState());

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
