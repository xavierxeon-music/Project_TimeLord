#include "StageModel.h"

#include "MainWidget.h"

Stage::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Data::Core()

{
   setHorizontalHeaderLabels({"index", "start position", "length", "start height"});
}

Stage::Model::Items Stage::Model::create(const Data::Identifier& identifier)
{
   Items items;
   items.indexItem = new QStandardItem();
   {
      QString name = QString::number(identifier.stageIndex + 1);
      while (3 != name.length())
         name = QString("0") + name;

      items.indexItem->setText(name);
      items.indexItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      items.indexItem->setEditable(false);
   }

   items.startPosItem = new QStandardItem();
   {
      items.startPosItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      items.startPosItem->setEditable(false);
   }

   items.lengthItem = new QStandardItem();
   {
      items.lengthItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      items.lengthItem->setData(QVariant::fromValue(Data::Target::StageLength), Data::Role::Target);
   }

   items.startHeigthItem = new QStandardItem();
   {
      items.startHeigthItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      items.startHeigthItem->setData(QVariant::fromValue(Data::Target::StageStartHeight), Data::Role::Target);
   }

   invisibleRootItem()->appendRow({items.indexItem, items.startPosItem, items.lengthItem, items.startHeigthItem});

   return items;
}

Stage::Model::Items Stage::Model::find(const int& row)
{
   Items items;

   items.indexItem = invisibleRootItem()->child(row, 0);
   items.startPosItem = invisibleRootItem()->child(row, 1);
   items.lengthItem = invisibleRootItem()->child(row, 2);
   items.startHeigthItem = invisibleRootItem()->child(row, 3);

   return items;
}

void Stage::Model::rebuildModel(Data::Identifier identifier)
{
   clear();
   setHorizontalHeaderLabels({"index", "start position", "length", "start height"});

   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   Data::Identifier stageIdentifier = identifier;

   for (uint8_t stageIndex = 0; stageIndex < polyRamp->stageCount(); stageIndex++)
   {
      stageIdentifier.stageIndex = stageIndex;
      Items items = create(stageIdentifier);

      QString name = QString::number(stageIndex + 1);
      while (3 != name.length())
         name = QString("0") + name;

      items.indexItem->setText(name);

      update(polyRamp, stageIndex);
   }

   callOnAllInstances(&Core::modelHasChanged, identifier);
}

void Stage::Model::update(PolyRamp* polyRamp, const uint8_t& itemStageIndex)
{
   uint32_t startPos = 0;
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      Items items = find(row);

      const Data::Identifier identifier = items.indexItem->data(Data::Role::Identifier).value<Data::Identifier>();

      const uint8_t nextIndex = (identifier.stageIndex + 1 < polyRamp->stageCount()) ? identifier.stageIndex + 1 : 0;
      const uint8_t startHeight = polyRamp->getStageStartHeight(identifier.stageIndex);
      const uint8_t endHeight = polyRamp->getStageStartHeight(nextIndex);
      const uint8_t length = polyRamp->getStageLength(identifier.stageIndex);

      QString posText = QString::number(startPos);
      items.startPosItem->setText(posText);

      if (0 == length && 0 != nextIndex)
         items.startPosItem->setIcon(QIcon(":/TrendStep.svg"));
      else if (startHeight < endHeight)
         items.startPosItem->setIcon(QIcon(":/TrendRise.svg"));
      else if (startHeight > endHeight)
         items.startPosItem->setIcon(QIcon(":/TrendFall.svg"));
      else
         items.startPosItem->setIcon(QIcon(":/TrendStable.svg"));

      if (identifier.stageIndex == itemStageIndex)
      {
         QString lengthText = QString::number(length);
         if (identifier.stageIndex + 1 == polyRamp->stageCount())
            lengthText += "*";
         items.lengthItem->setText(lengthText);

         const QString startHeightText = QString::number(startHeight);
         items.startHeigthItem->setText(startHeightText);
      }

      startPos += polyRamp->getStageLength(identifier.stageIndex);
   }
}

bool Stage::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   if (Qt::EditRole != role)
      return QStandardItemModel::setData(index, value, role);

   const QVariant targetData = data(index, Data::Role::Target);
   if (targetData.isNull())
      return QStandardItemModel::setData(index, value, role);

   const Data::Identifier identifier = data(index, Data::Role::Identifier).value<Data::Identifier>();
   PolyRamp* polyRamp = getPolyRamp(identifier);

   QVariant targeValue = value;
   const Data::Target::Value target = targetData.value<Data::Target::Value>();

   if (Data::Target::StageStartHeight == target)
   {
      const uint8_t height = value.toInt();
      polyRamp->setStageStartHeight(identifier.stageIndex, height);
      callOnAllInstances(&Core::modelHasChanged, identifier);
   }
   else if (Data::Target::StageLength == target)
   {
      QString length = value.toString().replace("*", "");
      if (PolyRamp::LengthStatus::Changed != polyRamp->setStageLength(identifier.stageIndex, length.toInt(), !getLockGraphSize()))
      {
         targeValue = polyRamp->getStageLength(identifier.stageIndex); // UNDO
      }
      else
      {
         if (identifier.stageIndex + 1 == polyRamp->stageCount())
            length += "*";
         targeValue = length;
         callOnAllInstances(&Core::modelHasChanged, identifier);
      }
   }

   bool result = QStandardItemModel::setData(index, targeValue, role);
   update(polyRamp, identifier.stageIndex);
   return result;
}
