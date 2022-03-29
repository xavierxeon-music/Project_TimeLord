#include "StageModel.h"

#include "MainWidget.h"

StageModel::StageModel(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , DataCore(mainWidget)

{
   setHorizontalHeaderLabels({"index", "position", "length", "height"});
}

void StageModel::rebuild(const Model::Provider provider, const uint8_t& graphIndex, bool lengthChanged)
{
   clear();
   setHorizontalHeaderLabels({"index", "position", "length", "height"});

   PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
   if (!polyRamp)
      return;

   QStandardItem* parentItem = new QStandardItem(polyRamp->stageCount());
   parentItem->setEditable(false);

   for (uint8_t stageIndex = 0; stageIndex < polyRamp->stageCount(); stageIndex++)
   {
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

      QStandardItem* posItem = new QStandardItem();
      {
         posItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         posItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         posItem->setData(QVariant::fromValue(stageIndex), Model::Role::StageIndex);
         posItem->setEditable(false);
      }

      QStandardItem* lengthItem = new QStandardItem();
      {
         lengthItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         lengthItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         lengthItem->setData(QVariant::fromValue(stageIndex), Model::Role::StageIndex);
         lengthItem->setData(QVariant::fromValue(Model::Target::StageLength), Model::Role::Target);
      }

      QStandardItem* startItem = new QStandardItem();
      {
         startItem->setData(QVariant::fromValue(provider), Model::Role::Provider);
         startItem->setData(QVariant::fromValue(graphIndex), Model::Role::GraphIndex);
         startItem->setData(QVariant::fromValue(stageIndex), Model::Role::StageIndex);
         startItem->setData(QVariant::fromValue(Model::Target::StageHeight), Model::Role::Target);
      }

      invisibleRootItem()->appendRow({indexItem, posItem, lengthItem, startItem});
      update(polyRamp, stageIndex);
   }

   if (lengthChanged)
      emit signalGraphLengthChanged(provider, graphIndex);
}

void StageModel::update(PolyRamp* polyRamp, const uint8_t& itemStageIndex)
{
   uint32_t startPos = 0;
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* nameItem = invisibleRootItem()->child(row, 0);
      const uint8_t stageIndex = nameItem->data(Model::Role::StageIndex).value<uint8_t>();

      const uint8_t nextIndex = (stageIndex + 1 < polyRamp->stageCount()) ? stageIndex + 1 : 0;
      const uint8_t startHeight = polyRamp->getStageStartHeight(stageIndex);
      const uint8_t endHeight = polyRamp->getStageStartHeight(nextIndex);
      const uint8_t length = polyRamp->getStageLength(stageIndex);

      QStandardItem* posItem = invisibleRootItem()->child(row, 1);
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

      if (stageIndex == itemStageIndex)
      {
         QStandardItem* lengthItem = invisibleRootItem()->child(row, 2);
         QString lengthText = QString::number(length);
         if (stageIndex + 1 == polyRamp->stageCount())
            lengthText += "*";
         lengthItem->setText(lengthText);

         QStandardItem* startItem = invisibleRootItem()->child(row, 3);
         const QString startHeightText = QString::number(startHeight);
         startItem->setText(startHeightText);
      }

      startPos += polyRamp->getStageLength(stageIndex);
   }
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

   PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
   const uint8_t stageIndex = data(index, Model::Role::StageIndex).value<uint8_t>();

   QVariant targeValue = value;

   const Model::Target target = targetData.value<Model::Target>();
   if (Model::Target::StageHeight == target)
   {
      const uint8_t height = value.toInt();
      polyRamp->setStageStartHeight(stageIndex, height);
   }
   else if (Model::Target::StageLength == target)
   {
      QString length = value.toString().replace("*", "");
      if (PolyRamp::LengthStatus::Changed != polyRamp->setStageLength(stageIndex, length.toInt(), !lockGraphSize))
      {
         targeValue = polyRamp->getStageLength(stageIndex); // UNDO
      }
      else
      {
         if (stageIndex + 1 == polyRamp->stageCount())
            length += "*";
         targeValue = length;
         emit signalGraphLengthChanged(provider, graphIndex);
      }
   }

   bool result = QStandardItemModel::setData(index, targeValue, role);
   update(polyRamp, stageIndex);
   return result;
}
