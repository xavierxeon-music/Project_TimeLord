#include "PointModel.h"

#include "MainWidget.h"

PointModel::PointModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)
   , portIndex(0)
   , pointIndex(0)
{
}

void PointModel::slotPortChanged(const Provider provider, const uint8_t& index)
{
   portProvider = provider;
   portIndex = index;
   update();
}

void PointModel::slotInsertPoint()
{
   Graph& currentGraph = graph(Provider::DaisyPatch, portIndex);
   currentGraph.addStage(255, 1, pointIndex, true);

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
