#include "PointModel.h"

#include "MainWidget.h"

PointModel::PointModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)
   , portIndex(0)
   , pointIndex(0)
{
}

void PointModel::slotPortChanged(const Model::Provider provider, const uint8_t& index)
{
   portProvider = provider;
   portIndex = index;
   update();
}

void PointModel::slotInsertPoint()
{
   Graph* graph = getGraph(portProvider, portIndex);
   graph->addStage(255, 1, pointIndex, true);

   update();
}

void PointModel::slotRemovePoint()
{
   qDebug() << __FUNCTION__;
}

void PointModel::slotMoveBack()
{
   qDebug() << __FUNCTION__;
}

void PointModel::slotMoveForward()
{
   qDebug() << __FUNCTION__;
}

void PointModel::slotPointSelected(const uint8_t& index)
{
   pointIndex = index;
}

void PointModel::update()
{
   clear();
   Graph* graph = getGraph(portProvider, portIndex);

   QStandardItem* parentItem = new QStandardItem(graph->stageCount());
   parentItem->setEditable(false);

   for (uint8_t index = 0; index < graph->stageCount(); index++)
   {
      const Graph::Stage& stage = graph->getStage(index);
   }
}
