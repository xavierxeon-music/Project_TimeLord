#ifndef MainWidgetH
#define MainWidgetH

#include "DataCore.h"
#include <QWidget>

#include <QSplitter>
#include <QStatusBar>

#include <Blocks/PolyRamp.h>
#include <RootStorage.h>

#include "PolyLineWidget.h"
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
   const uint8_t& getBankIndex() const;
   void setBankIndex(const uint8_t& newBankIndex);

public slots:
   void slotLoadFromFile();
   void slotSaveToFile();
   void slotSaveNewFile();
   void slotPushToServer();

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

   uint8_t bankIndex;
};

#endif // MainWidgetH
