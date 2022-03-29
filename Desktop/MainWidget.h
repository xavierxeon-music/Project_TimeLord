#ifndef MainWidgetH
#define MainWidgetH

#include <Remember.h>
#include <QWidget>

#include <QSplitter>
#include <QStatusBar>

#include <Blocks/PolyRamp.h>
#include <FileStorage.h>
#include <MidiBridge.h>

#include "PolyLineModel.h"
#include "PolyLineWidget.h"
#include "PolyRampModel.h"
#include "PolyRampVisu.h"
#include "PolyRampWidget.h"
#include "StageModel.h"
#include "StageWidget.h"

class AudioDeviceGraph;

class MainWidget : public QWidget, public Remember::Root
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
   void slotSaveToDaisy();

private:
   friend class Data::Core;

private: // things to remeber
   using PolyRampList_ = Remember::RefArray<PolyRamp, 16>;

private slots:
   void slotCheckDataModified();

private:
   void loadLastFile();
   void loadInternal(const QString& fileName);
   void saveInternal(const QString& fileName);
   void updateWindowTitle(const QString& fileName);
   void closeEvent(QCloseEvent* ce) override;

private:
   PolyRampList_ polyRamps;
   AudioDeviceGraph* audioDevice;

   MidiBridge midiBridge;
   FileStorage fileStorageDaisy;
   FileStorage fileStorageDevice;

   QSplitter* splitter;
   QStatusBar* statusBar;

   PolyRampModel* polyRampModel;
   PolyRampVisu* polyRampVisu;
   PolyRampWidget* polyRampWidget;

   StageModel* stageModel;
   StageWidget* stageWidget;

   PolyLine::Model* polyLineModel;
   PolyLine::Widget* polyLineWidget;
};

#endif // MainWidgetH
