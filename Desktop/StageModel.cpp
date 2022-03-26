#include "StageModel.h"

#include "MainWidget.h"

StageModel::StageModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)
   , provider(Model::Provider::DaisyPatch)
   , graphIndex(0)
   , selectedStageIndex(0)
{
}

void StageModel::slotPortChanged(const Model::Provider newProvider, const uint8_t& newGraphIndex)
{
   provider = newProvider;
   graphIndex = newGraphIndex;
   update();
}

void StageModel::slotInsertPoint()
{
   Graph* graph = getGraph(provider, graphIndex);
   if (!graph->addStage(255, selectedStageIndex, graphIndex, true))
      return;

   update();
}

void StageModel::slotRemovePoint()
{
   qDebug() << __FUNCTION__;
}

void StageModel::slotMoveBack()
{
   qDebug() << __FUNCTION__;
}

void StageModel::slotMoveForward()
{
   qDebug() << __FUNCTION__;
}

void StageModel::slotPointSelected(const uint8_t& index)
{
   selectedStageIndex = index;
}

void StageModel::update()
{
   clear();
   setHorizontalHeaderLabels({"index", "start", "length"});

   Graph* graph = getGraph(provider, graphIndex);

   QStandardItem* parentItem = new QStandardItem(graph->stageCount());
   parentItem->setEditable(false);

   for (uint8_t itemStageIndex = 0; itemStageIndex < graph->stageCount(); itemStageIndex++)
   {
      QStandardItem* indexItem = new QStandardItem();
      {
         QString name = QString::number(itemStageIndex + 1);
         while (3 != name.length())
            name = QString("0") + name;

         indexItem->setText(name);
         indexItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         indexItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         indexItem->setData(QVariant::fromValue(itemStageIndex), Model::Role::StageIndex);
         indexItem->setEditable(false);
      }

      QStandardItem* startItem = new QStandardItem("start");
      QStandardItem* lengthItem = new QStandardItem("length");

      invisibleRootItem()->appendRow({indexItem, startItem, lengthItem});
   }
}
