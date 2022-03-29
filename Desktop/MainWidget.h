#ifndef MainWidgetH
#define MainWidgetH

#include <Remember.h>
#include <QWidget>

#include <QSplitter>
#include <QStatusBar>

#include <Blocks/Graph.h>
#include <FileStorage.h>
#include <MidiBridge.h>

#include "GraphVisuWidget.h"
#include "PolyRampModel.h"
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
   friend class DataCore;

private: // things to remeber
   using GraphList_ = Remember::RefArray<Graph, 16>;

private slots:
   void slotCheckDataModified();

private:
   void loadLastFile();
   void loadInternal(const QString& fileName);
   void saveInternal(const QString& fileName);
   void updateWindowTitle(const QString& fileName);
   void closeEvent(QCloseEvent* ce) override;

private:
   GraphList_ graphs;
   AudioDeviceGraph* audioDevice;

   MidiBridge midiBridge;
   FileStorage fileStorageDaisy;
   FileStorage fileStorageDevice;

   QSplitter* splitter;
   QStatusBar* statusBar;

   PolyRampWidget* polyRampWidget;
   PolyRampModel* polyRampModel;

   StageWidget* stageWidget;
   StageModel* stageModel;

   GraphVisuWidget* graphVisuWidget;
};

#endif // MainWidgetH
