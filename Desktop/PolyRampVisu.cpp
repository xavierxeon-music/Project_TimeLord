#include "PolyRampVisu.h"

#include <QAction>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QTimer>

#include "MainWidget.h"

PolyRampVisu::PolyRampVisu(MainWidget* mainWidget)
   : AbstractWidget(mainWidget)
   , graphicsView(nullptr)
   , stageMap()
   , selectedProvider(Model::Provider::None)
   , selectedGraphIndex(0)
   , zoomLevel(10)
{
   setFixedHeight(200);
   toolBar->addAction(QIcon(":/ZoomIn.svg"), "Zoom In", this, &PolyRampVisu::slotZoomIn);
   toolBar->addAction(QIcon(":/ZoomOut.svg"), "Zoom Out", this, &PolyRampVisu::slotZoomOut);

   QGraphicsScene* scene = new QGraphicsScene(this);
   scene->setSceneRect(0, 0, 150, 150);
   graphicsView = new QGraphicsView(scene, this);
   addPayload(graphicsView);

   QTimer* updateTimer = new QTimer(this);
   connect(updateTimer, &QTimer::timeout, this, &PolyRampVisu::slotUpdate);
   updateTimer->start(500);

   QPen whitePen(QColor(255, 255, 255));
   graphicsView->scene()->addLine(0, 0, 0, 150, whitePen); // to force height even withou graph data
}

void PolyRampVisu::slotGraphSelected(const Model::Provider& newProvider, const uint8_t& newGraphIndex)
{
   selectedProvider = newProvider;
   selectedGraphIndex = newGraphIndex;
   slotUpdate();
}

void PolyRampVisu::slotUpdate()
{
   static const QPen blackPen(QColor(0, 0, 0), 2);
   static const QPen grayPen(QColor(200, 200, 200));

   Graph* selectedGraph = getGraph(selectedProvider, selectedGraphIndex);

   auto drawGraph = [&](Graph* graph)
   {
      if (!graph || 0 == graph->stageCount())
         return;

      Stage::List& stageList = stageMap[graph];
      while (stageList.size() < graph->stageCount()) // add lines
      {
         Stage stage;
         stage.lineItem = new QGraphicsLineItem();
         graphicsView->scene()->addItem(stage.lineItem);

         stageList.append(stage);
      }

      while (stageList.size() > graph->stageCount()) // remove lines
      {
         Stage stage = stageList.takeLast();
         delete stage.lineItem;
      }

      uint16_t offsetY = 10;
      uint32_t startX = 5;

      for (uint8_t startIndex = 0; startIndex < graph->stageCount(); startIndex++)
      {
         uint32_t stageLength = graph->getStageLength(startIndex);
         uint8_t endIndex = (startIndex + 1 < graph->stageCount()) ? startIndex + 1 : 0;

         uint32_t startY = 128 - (0.5 * graph->getStageStartHeight(startIndex));
         uint32_t endY = 128 - (0.5 * graph->getStageStartHeight(endIndex));

         uint32_t endX = (endIndex == 0) ? graph->getLength() : startX + stageLength;

         QGraphicsLineItem* lineItem = stageList[startIndex].lineItem;
         lineItem->setLine(zoomLevel * startX, offsetY + startY, zoomLevel * endX, offsetY + endY);
         lineItem->setPen((graph == selectedGraph) ? blackPen : grayPen);
         lineItem->setZValue((graph == selectedGraph) ? 1.0 : 0.0);

         startX = endX;
      }
   };

   const PoviderNameMap& nameMap = getProviderNames();
   for (PoviderNameMap::const_iterator it = nameMap.constBegin(); it != nameMap.constEnd(); it++)
   {
      const Model::Provider provider = it.key();

      for (uint8_t graphIndex = 0; graphIndex < 16; graphIndex++)
      {
         Graph* graph = getGraph(provider, graphIndex);
         drawGraph(graph);
      }
   }

   drawGraph(selectedGraph);

   QRectF contentRect = graphicsView->scene()->itemsBoundingRect();
   contentRect.setHeight(150);
   graphicsView->scene()->setSceneRect(contentRect);
}

void PolyRampVisu::slotZoomIn()
{
   if (255 == zoomLevel)
      return;

   zoomLevel++;
   slotUpdate();
}

void PolyRampVisu::slotZoomOut()
{
   if (1 == zoomLevel)
      return;

   zoomLevel--;
   slotUpdate();
}