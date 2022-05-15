#include "MainWidget.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QJsonArray>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>

#include <AppSettings.h>
#include <FileSettings.h>

MainWidget::MainWidget()
   : QWidget(nullptr)
   , Core::Interface()
   , splitter(nullptr)
   , statusBar(nullptr)
   , actions{nullptr, nullptr, nullptr}
   , visuWidget(nullptr)
   , bankWidget(nullptr)
   , rampWidget(nullptr)
   , stageWidget(nullptr)
{
   setWindowTitle("Time Lord UI[*]");
   setMinimumSize(1400, 900);

   createRampDevice(this);

   statusBar = new QStatusBar(this);
   statusBar->setSizeGripEnabled(true);

   loadLastFile();

   actions.loadFromFile = new QAction(QIcon(":/LoadFromFile.svg"), "Load From File", this);
   connect(actions.loadFromFile, &QAction::triggered, this, &MainWidget::slotLoadFromFile);

   actions.saveToFile = new QAction(QIcon(":/SaveToFile.svg"), "Save To File", this);
   connect(actions.saveToFile, &QAction::triggered, this, &MainWidget::slotSaveToFile);
   actions.saveToFile->setShortcut(QKeySequence(QKeySequence::Save));

   actions.saveNewFile = new QAction(QIcon(":/SaveNewFile.svg"), "Save To New File", this);
   connect(actions.saveNewFile, &QAction::triggered, this, &MainWidget::slotSaveNewFile);

   visuWidget = new Visu::Widget(this);
   bankWidget = new Bank::Widget(this);
   rampWidget = new Ramp::Widget(this);
   stageWidget = new Stage::Widget(this);

   splitter = new QSplitter(Qt::Horizontal, this);
   splitter->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   splitter->setObjectName("ModelSplitter");
   splitter->addWidget(bankWidget);
   splitter->addWidget(rampWidget);
   splitter->addWidget(stageWidget);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(visuWidget);
   masterLayout->addWidget(splitter);
   masterLayout->addWidget(statusBar);

   AppSettings widgetSettings("MainWidget");
   restoreGeometry(widgetSettings.bytes("Geometry"));
   splitter->restoreState(widgetSettings.bytes("StateModel"));

   QTimer* modfifiedCheckTimer = new QTimer(this);
   connect(modfifiedCheckTimer, &QTimer::timeout, this, &MainWidget::slotCheckDataModified);
   modfifiedCheckTimer->start(1000);
}

const MainWidget::FileActions& MainWidget::getFileActions() const
{
   return actions;
}

const Target::ServerActions& MainWidget::getServerActions() const
{
   return target->getServerActions();
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

void MainWidget::slotCheckDataModified()
{
   if (isModified)
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

   //TODO

   updateWindowTitle(fileName);
   unsetModified();

   Core::Identifier dummy;
   callOnAllInstances(&Interface::rebuildModel, dummy);

   statusBar->showMessage("file loaded", 2000);
}

void MainWidget::saveInternal(const QString& fileName)
{
   QJsonArray bankArray;

   for (uint8_t bankIndex = 0; bankIndex < getBankCount(); bankIndex++)
   {
      const Core::Identifier bankIdentifier(bankIndex);
      const Bank::Content* bank = getBank(bankIdentifier);
      if (!bank)
         continue;

      QJsonObject bankObject;
      bankObject["index"] = bankIndex;
      bankObject["bpm"] = bank->getBeatsPerMinute();
      bankObject["names"] = bank->writeNames();
      bankObject["content"] = bank->writeRamps();
      bankArray.append(bankObject);
   }

   FileSettings settings;
   settings.write("banks", bankArray);

   // TODO

   updateWindowTitle(fileName);
   unsetModified();

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
