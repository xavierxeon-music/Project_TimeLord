#ifndef MainWidgetH
#define MainWidgetH

#include "Core.h"
#include <QWidget>

#include <QSplitter>
#include <QStatusBar>

#include <Blocks/PolyRamp.h>
#include <RootStorage.h>

#include "BankWidget.h"
#include "RampWidget.h"
#include "StageWidget.h"
#include "VisuWidget.h"

#include "Target.h"

class MainWidget : public QWidget, public Core::Interface
{
   Q_OBJECT

public:
   struct FileActions
   {
      QAction* newFile;
      QAction* loadFromFile;
      QAction* saveToFile;
      QAction* saveNewFile;
   };

public:
   MainWidget();

public:
   const FileActions& getFileActions() const;
   const Target::ServerActions& getServerActions() const;

private slots:
   void slotNewFile();
   void slotLoadFromFile();
   void slotSaveToFile();
   void slotSaveNewFile();
   void slotCheckDataModified();
   void slotCaptureStates();

private:
   void loadLastFile();
   void loadInternal(const QString& fileName);
   void saveInternal(const QString& fileName);
   void updateWindowTitle(const QString& fileName);
   void closeEvent(QCloseEvent* ce) override;

private:
   QSplitter* splitter;
   QStatusBar* statusBar;

   FileActions actions;

   Visu::Widget* visuWidget;
   Bank::Widget* bankWidget;
   Ramp::Widget* rampWidget;
   Stage::Widget* stageWidget;
};

#endif // MainWidgetH
