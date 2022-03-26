#include "PointModel.h"

#include "MainWidget.h"

PointModel::PointModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)
   , provider(Model::Provider::DaisyPatch)
   , graphIndex(0)
   , selectedStageIndex(0)
{
}

void PointModel::slotPortChanged(const Model::Provider newProvider, const uint8_t& newGraphIndex)
{
   provider = newProvider;
   graphIndex = newGraphIndex;
   update();
}

void PointModel::slotInsertPoint()
{
   Graph* graph = getGraph(provider, graphIndex);
   graph->addStage(255, selectedStageIndex, graphIndex, true);

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
   selectedStageIndex = index;
}

void PointModel::update()
{
   clear();
   Graph* graph = getGraph(provider, graphIndex);

   QStandardItem* parentItem = new QStandardItem(graph->stageCount());
   parentItem->setEditable(false);

   for (uint8_t itemStageIndex = 0; itemStageIndex < graph->stageCount(); itemStageIndex++)
   {
      const Graph::Stage& stage = graph->getStage(itemStageIndex);

      QStandardItem* nameItem = new QStandardItem();
      {
         QString name = QString::number(itemStageIndex + 1);
         if (1 == name.length())
            name = QString("0") + name;

         nameItem->setText("Port " + name);
         nameItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         nameItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         nameItem->setData(QVariant::fromValue(itemStageIndex), Model::Role::StageIndex);
         nameItem->setEditable(false);
      }
   }
}
