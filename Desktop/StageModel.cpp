#include "StageModel.h"

#include "MainWidget.h"

StageModel::StageModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)

{
   setHorizontalHeaderLabels({"index", "height", "length", "position"});
}

void StageModel::rebuild(const Model::Provider provider, const uint8_t& graphIndex, bool lengthChanged)
{
   clear();
   setHorizontalHeaderLabels({"index", "height", "length", "position"});

   Graph* graph = getGraph(provider, graphIndex);
   if (!graph)
      return;

   QStandardItem* parentItem = new QStandardItem(graph->stageCount());
   parentItem->setEditable(false);

   uint32_t startPos = 0;
   for (uint8_t stageIndex = 0; stageIndex < graph->stageCount(); stageIndex++)
   {
      const uint8_t nextIndex = (stageIndex + 1 < graph->stageCount()) ? stageIndex + 1 : 0;
      const uint8_t startHeight = graph->getStageStartHeight(stageIndex);
      const uint8_t endHeight = graph->getStageStartHeight(nextIndex);
      const uint8_t length = graph->getStageLength(stageIndex);

      QStandardItem* indexItem = new QStandardItem();
      {
         QString name = QString::number(stageIndex + 1);
         while (3 != name.length())
            name = QString("0") + name;

         indexItem->setText(name);
         indexItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         indexItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         indexItem->setData(QVariant::fromValue(stageIndex), Model::Role::StageIndex);
         indexItem->setEditable(false);
      }

      QStandardItem* startItem = new QStandardItem();
      {
         const QString startHeightText = QString::number(startHeight);
         startItem->setText(startHeightText);
         startItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         startItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         startItem->setData(QVariant::fromValue(stageIndex), Model::Role::StageIndex);
         startItem->setData(QVariant::fromValue(Model::Target::StageHeight), Model::Role::Target);
      }

      QStandardItem* lengthItem = new QStandardItem();
      {
         QString lengthText = QString::number(length);
         if (stageIndex + 1 == graph->stageCount())
            lengthText += "*";
         lengthItem->setText(lengthText);
         lengthItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         lengthItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         lengthItem->setData(QVariant::fromValue(stageIndex), Model::Role::StageIndex);
         lengthItem->setData(QVariant::fromValue(Model::Target::StageLength), Model::Role::Target);
      }

      QStandardItem* posItem = new QStandardItem();
      {
         QString posText = QString::number(startPos);
         posItem->setText(posText);

         if (0 == length && 0 != nextIndex)
            posItem->setIcon(QIcon(":/TrendDrop.svg"));
         else if (startHeight < endHeight)
            posItem->setIcon(QIcon(":/TrendUp.svg"));
         else if (startHeight > endHeight)
            posItem->setIcon(QIcon(":/TrendDown.svg"));
         else
            posItem->setIcon(QIcon(":/TrendStable.svg"));

         posItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         posItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         posItem->setData(QVariant::fromValue(stageIndex), Model::Role::StageIndex);
         posItem->setEditable(false);
      }
      startPos += graph->getStageLength(stageIndex);

      invisibleRootItem()->appendRow({indexItem, startItem, lengthItem, posItem});
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
      QString length = value.toString().replace("*", "");
      if (Graph::LengthStatus::Changed != graph->setStageLength(stageIndex, length.toInt(), !lockGraphSize))
      {
         targeValue = graph->getStageLength(stageIndex); // UNDO
      }
      else
      {
         if (stageIndex + 1 == graph->stageCount())
            length += "*";
         targeValue = length;
         emit signalGraphLengthChanged(provider, graphIndex);
      }
   }

   bool result = QStandardItemModel::setData(index, targeValue, role);
   return result;
}
