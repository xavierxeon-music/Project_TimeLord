#include "GraphVisuWidget.h"

#include <QAction>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QTimer>

#include "MainWidget.h"

GraphVisuWidget::GraphVisuWidget(MainWidget* mainWidget)
   : AbstractWidget(mainWidget)
   , graphicsView(nullptr)
   , stageList()
   , graph(nullptr)
{
   toolBar->addAction(QIcon(":/ZoomIn.svg"), "Zoom In");
   toolBar->addAction(QIcon(":/ZoomOut.svg"), "Zoom Out");

   QGraphicsScene* scene = new QGraphicsScene(this);
   graphicsView = new QGraphicsView(scene, this);
   addPayload(graphicsView);

   QTimer* updateTimer = new QTimer(this);
   connect(updateTimer, &QTimer::timeout, this, &GraphVisuWidget::slotUpdate);
   updateTimer->start(500);
}

void GraphVisuWidget::slotGraphSelected(const Model::Provider& newProvider, const uint8_t& newGraphIndex)
{
   graph = getGraph(newProvider, newGraphIndex);
   slotUpdate();
}

void GraphVisuWidget::slotUpdate()
{
   if (!graph || 0 == graph->stageCount())
      return;

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

   uint32_t startX = 0;
   for (uint8_t startIndex = 0; startIndex < graph->stageCount(); startIndex++)
   {
      uint8_t stageLength = graph->getStageLength(startIndex);
      uint8_t endIndex = (startIndex + 1 < graph->stageCount()) ? startIndex + 1 : 0;

      uint8_t startY = 255 - graph->getStageStartHeight(startIndex);
      uint8_t endY = 255 - graph->getStageStartHeight(endIndex);

      uint32_t endX = (endIndex == 0) ? graph->getLength() : startX + stageLength;

      stageList[startIndex].lineItem->setLine(startX, startY, endX, endY);
      startX = endX;
   }
}
