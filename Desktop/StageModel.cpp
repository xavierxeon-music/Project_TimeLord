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

   if (Graph::LengthStatus::Changed != graph->addStage(255, selectedStageIndex, graphIndex, true))
      return;

   emit signalGraphLengthChanged(graphIndex);
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
   setHorizontalHeaderLabels({"index", "height", "length"});

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

      QStandardItem* startItem = new QStandardItem();
      {
         const QString startHeight = QString::number(graph->getStageStartHeight(itemStageIndex));
         startItem->setText(startHeight);
         startItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         startItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         startItem->setData(QVariant::fromValue(itemStageIndex), Model::Role::StageIndex);
         startItem->setData(QVariant::fromValue(Model::Target::StageHeight), Model::Role::Target);
      }

      QStandardItem* lengthItem = new QStandardItem();
      {
         const QString length = QString::number(graph->getStageLength(itemStageIndex));
         lengthItem->setText(length);
         lengthItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         lengthItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         lengthItem->setData(QVariant::fromValue(itemStageIndex), Model::Role::StageIndex);
         lengthItem->setData(QVariant::fromValue(Model::Target::StageLength), Model::Role::Target);
      }

      invisibleRootItem()->appendRow({indexItem, startItem, lengthItem});
   }
}

bool StageModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
   bool result = QStandardItemModel::setData(index, value, role);
   if (Qt::EditRole != role)
      return result;

   const QVariant targetData = data(index, Model::Role::Target);
   if (targetData.isNull())
      return result;

   const Model::Provider provider = data(index, Model::Role::Provider).value<Model::Provider>();
   const uint8_t graphIndex = data(index, Model::Role::GraphIndex).value<uint8_t>();

   Graph* graph = getGraph(provider, graphIndex);

   const uint8_t stageIndex = data(index, Model::Role::StageIndex).value<uint8_t>();

   const Model::Target target = targetData.value<Model::Target>();
   if (Model::Target::StageHeight == target)
   {
      const uint8_t height = value.toInt();
      graph->setStageStartHeight(stageIndex, height);
   }
   else if (Model::Target::StageLength == target)
   {
      const uint8_t length = value.toInt();
      graph->setStageLength(stageIndex, length);
   }

   return result;
}
