#ifndef MainWidgetH
#define MainWidgetH

#include <Remember.h>
#include <QWidget>

#include <QSplitter>

#include <Blocks/Graph.h>
#include <FileStorage.h>
#include <MidiBridge.h>

#include "DeviceModel.h"
#include "DeviceWidget.h"
#include "GraphWidget.h"
#include "PointModel.h"
#include "PointWidget.h"

class GraphAudioDevice;

class MainWidget : public QWidget, public Remember::Root
{
   Q_OBJECT
public: // things to remeber
   using GraphList_ = Remember::RefArray<Graph, 16>;

public:
   MainWidget();

public slots:
   void slotNewFile();
   void slotLoadFromFile();
   void slotSaveToFile();
   void slotSaveToDaisy();

private:
   friend class DataCore;

private:
   void loadedFromDaisy();
   void updateUI();
   void closeEvent(QCloseEvent* ce) override;

private:
   GraphList_ graphs;
   GraphAudioDevice* graphAudioDevice;

   MidiBridge midiBridge;
   FileStorage fileStorageDaisy;
   FileStorage fileStorageDevice;

   QSplitter* splitter;

   DeviceWidget* deviceWidget;
   DeviceModel* deviceModel;

   PointWidget* pointWidget;
   PointModel* pointModel;

   GraphWidget* graphWidget;
};

#endif // MainWidgetH
