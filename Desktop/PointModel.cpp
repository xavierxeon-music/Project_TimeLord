#include "PointModel.h"

#include "MainWidget.h"

PointModel::PointModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)
   , portIndex(0)
   , pointIndex(0)
{
}

void PointModel::slotPortChanged(const uint8_t& index)
{
   portIndex = index;
   update();
}

void PointModel::slotInsertPoint()
{
   TimeLord::GraphList_& graphList = getGraphList();
   Graph& graph = graphList[portIndex];
   graph.addStage(255, 1, pointIndex, true);

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
}
