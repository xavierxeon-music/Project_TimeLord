#ifndef MainWidgetH
#define MainWidgetH

#include "DataCore.h"
#include <QWidget>

#include <QSplitter>
#include <QStatusBar>

#include <Blocks/PolyRamp.h>
#include <FileStorage.h>

#include "PolyLineWidget.h"
#include "RampDeviceRaspi.h"
#include "RampVisu.h"
#include "RampWidget.h"
#include "StageWidget.h"

class MainWidget : public QWidget, public Data::Core
{
   Q_OBJECT

public:
   MainWidget();

public:
   void forceRebuildModels();

public slots:
   void slotLoadFromFile();
   void slotSaveToFile();
   void slotSaveNewFile();
   void slotSaveToRaspi();
   void slotEnableMidiOutput(bool enabled);

private slots:
   void slotCheckDataModified();

private:
   void loadLastFile();
   void loadInternal(const QString& fileName);
   void saveInternal(const QString& fileName);
   void updateWindowTitle(const QString& fileName);
   void closeEvent(QCloseEvent* ce) override;

private:
   QSplitter* splitter;
   QStatusBar* statusBar;

   Ramp::Visu* polyRampVisu;
   Ramp::Widget* polyRampWidget;
   Stage::Widget* stageWidget;
   PolyLine::Widget* polyLineWidget;
};

#endif // MainWidgetH
