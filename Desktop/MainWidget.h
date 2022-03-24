#ifndef MainWidgetH
#define MainWidgetH

#include <Remember.h>
#include <QSplitter>

#include <Blocks/Graph.h>
#include <FileStorage.h>
#include <MidiBridge.h>

#include "GraphWidget.h"
#include "PointModel.h"
#include "PointWidget.h"
#include "PortModel.h"
#include "PortWidget.h"

class GraphDevice;

class MainWidget : public QSplitter, public Remember::Root
{
   Q_OBJECT
public: // things to remeber
   using GraphList_ = Remember::RefArray<Graph, 16>;

public:
   MainWidget();

public slots:
   void slotLoadFromFile();
   void slotStartLoadFromdDaisy();
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
   GraphDevice* graphDevice;

   MidiBridge midiBridge;
   FileStorage fileStorageDaisy;
   FileStorage fileStorageDevice;

   PortWidget* portWidget;
   PortModel* portModel;

   PointWidget* pointWidget;
   PointModel* pointModel;

   GraphWidget* graphWidget;
};

#endif // MainWidgetH
