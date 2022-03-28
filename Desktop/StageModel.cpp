#include "StageModel.h"

#include "MainWidget.h"

StageModel::StageModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)

{
   setHorizontalHeaderLabels({"index", "height", "length"});
}

void StageModel::rebuild(const Model::Provider provider, const uint8_t& graphIndex, bool lengthChanged)
{
   clear();
   setHorizontalHeaderLabels({"index", "height", "length"});

   Graph* graph = getGraph(provider, graphIndex);
   if (!graph)
      return;

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

   if (lengthChanged)
      emit signalGraphLengthChanged(provider, graphIndex);
}

bool StageModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
   if (Qt::EditRole != role)
      return QStandardItemModel::setData(index, value, role);

   const QVariant targetData = data(index, Model::Role::Target);
   if (targetData.isNull())
      return QStandardItemModel::setData(index, value, role);

   const Model::Provider provider = data(index, Model::Role::Provider).value<Model::Provider>();
   const uint8_t graphIndex = data(index, Model::Role::GraphIndex).value<uint8_t>();

   Graph* graph = getGraph(provider, graphIndex);
   const uint8_t stageIndex = data(index, Model::Role::StageIndex).value<uint8_t>();

   QVariant targeValue = value;

   const Model::Target target = targetData.value<Model::Target>();
   if (Model::Target::StageHeight == target)
   {
      const uint8_t height = value.toInt();
      graph->setStageStartHeight(stageIndex, height);
   }
   else if (Model::Target::StageLength == target)
   {
      const uint8_t length = value.toInt();
      if (Graph::LengthStatus::Changed != graph->setStageLength(stageIndex, length, !lockGraphSize))
      {
         targeValue = graph->getStageLength(stageIndex);
      }
      else
      {
         emit signalGraphLengthChanged(provider, graphIndex);
      }
   }

   bool result = QStandardItemModel::setData(index, targeValue, role);
   return result;
}
