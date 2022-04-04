#ifndef MainWidgetH
#define MainWidgetH

#include <Remember.h>
#include <QWidget>

#include <QSplitter>
#include <QStatusBar>

#include <Blocks/PolyRamp.h>
#include <FileStorage.h>

#include "PolyLineModel.h"
#include "PolyLineWidget.h"
#include "RampDeviceRaspi.h"
#include "RampModel.h"
#include "RampVisu.h"
#include "RampWidget.h"
#include "StageModel.h"
#include "StageWidget.h"

class MainWidget : public QWidget
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

private:
   friend class Data::Core;

private slots:
   void slotCheckDataModified();

private:
   void loadLastFile();
   void loadInternal(const QString& fileName);
   void saveInternal(const QString& fileName);
   void updateWindowTitle(const QString& fileName);
   void closeEvent(QCloseEvent* ce) override;

private:
   RampDevice::Raspi raspiDevice;

   QSplitter* splitter;
   QStatusBar* statusBar;

   Ramp::Model* polyRampModel;
   Ramp::Visu* polyRampVisu;
   Ramp::Widget* polyRampWidget;

   Stage::Model* stageModel;
   Stage::Widget* stageWidget;

   PolyLine::Model* polyLineModel;
   PolyLine::Widget* polyLineWidget;
};

#endif // MainWidgetH
