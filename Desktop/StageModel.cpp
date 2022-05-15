#include "StageModel.h"

#include "MainWidget.h"

Stage::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Core::Interface()

{
   setHorizontalHeaderLabels({"index", "start position", "length", "start height", "start height"});
}

Stage::Model::Items Stage::Model::create(const Core::Identifier& identifier)
{
   Items items;
   items.indexItem = new QStandardItem();
   {
      QString name = QString::number(identifier.stageIndex + 1);
      while (3 != name.length())
         name = QString("0") + name;

      items.indexItem->setText(name);
      items.indexItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.indexItem->setEditable(false);
   }

   items.startPosItem = new QStandardItem();
   {
      items.startPosItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.startPosItem->setEditable(false);
   }

   items.lengthItem = new QStandardItem();
   {
      items.lengthItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.lengthItem->setData(QVariant::fromValue(Core::Target::StageLength), Core::Role::Target);
   }

   items.startHeigthItem = new QStandardItem();
   {
      items.startHeigthItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.startHeigthItem->setData(QVariant::fromValue(Core::Target::StageStartHeight), Core::Role::Target);
   }

   items.endHeigthItem = new QStandardItem();
   {
      items.endHeigthItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.endHeigthItem->setData(QVariant::fromValue(Core::Target::StageEndHeight), Core::Role::Target);
   }

   invisibleRootItem()->appendRow({items.indexItem, items.startPosItem, items.lengthItem, items.startHeigthItem, items.endHeigthItem});

   return items;
}

Stage::Model::Items Stage::Model::find(const int& row)
{
   Items items;

   items.indexItem = invisibleRootItem()->child(row, 0);
   items.startPosItem = invisibleRootItem()->child(row, 1);
   items.lengthItem = invisibleRootItem()->child(row, 2);
   items.startHeigthItem = invisibleRootItem()->child(row, 3);
   items.endHeigthItem = invisibleRootItem()->child(row, 4);

   return items;
}

void Stage::Model::rebuildModel(Core::Identifier identifier)
{
   clear();
   setHorizontalHeaderLabels({"index", "start position", "length", "start height", "start height"});

   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   Core::Identifier stageIdentifier = identifier;

   for (uint8_t stageIndex = 0; stageIndex < polyRamp->getStageCount(); stageIndex++)
   {
      stageIdentifier.stageIndex = stageIndex;
      Items items = create(stageIdentifier);

      QString name = QString::number(stageIndex + 1);
      while (3 != name.length())
         name = QString("0") + name;

      items.indexItem->setText(name);

      update(polyRamp, stageIndex);
   }

   callOnAllInstances(&Interface::modelHasChanged, identifier);
}

void Stage::Model::update(PolyRamp* polyRamp, const uint8_t& itemStageIndex)
{
   uint32_t startPos = 0;
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      Items items = find(row);

      const Core::Identifier identifier = items.indexItem->data(Core::Role::Identifier).value<Core::Identifier>();

      const uint8_t startHeight = polyRamp->getStageStartHeight(identifier.stageIndex);
      const uint8_t endHeight = polyRamp->getStageEndHeight(identifier.stageIndex);
      const uint8_t length = polyRamp->getStageLength(identifier.stageIndex);

      QString posText = QString::number(startPos);
      items.startPosItem->setText(posText);

      if (0 == length)
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
         if (identifier.stageIndex + 1 == polyRamp->getStageCount())
            lengthText += "*";
         items.lengthItem->setText(lengthText);

         const QString startHeightText = QString::number(startHeight);
         items.startHeigthItem->setText(startHeightText);

         const QString endHeightText = QString::number(endHeight);
         items.endHeigthItem->setText(endHeightText);
      }

      startPos += polyRamp->getStageLength(identifier.stageIndex);
   }
}

bool Stage::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   if (Qt::EditRole != role)
      return QStandardItemModel::setData(index, value, role);

   const QVariant targetData = data(index, Core::Role::Target);
   if (targetData.isNull())
      return QStandardItemModel::setData(index, value, role);

   const Core::Identifier identifier = data(index, Core::Role::Identifier).value<Core::Identifier>();
   PolyRamp* polyRamp = getPolyRamp(identifier);

   QVariant targeValue = value;
   const Core::Target::Value target = targetData.value<Core::Target::Value>();

   if (Core::Target::StageStartHeight == target)
   {
      const uint8_t height = value.toInt();
      polyRamp->setStageStartHeight(identifier.stageIndex, height);
      callOnAllInstances(&Interface::modelHasChanged, identifier);

      setModified();
   }
   else if (Core::Target::StageEndHeight == target)
   {
      const uint8_t height = value.toInt();
      polyRamp->setStageEndHeight(identifier.stageIndex, height);
      callOnAllInstances(&Interface::modelHasChanged, identifier);

      setModified();
   }
   else if (Core::Target::StageLength == target)
   {
      QString length = value.toString().replace("*", "");
      if (PolyRamp::LengthStatus::Changed != polyRamp->setStageLength(identifier.stageIndex, length.toInt(), !getLockGraphSize()))
      {
         targeValue = polyRamp->getStageLength(identifier.stageIndex); // UNDO
      }
      else
      {
         if (identifier.stageIndex + 1 == polyRamp->getStageCount())
            length += "*";
         targeValue = length;
         callOnAllInstances(&Interface::modelHasChanged, identifier);

         setModified();
      }
   }

   bool result = QStandardItemModel::setData(index, targeValue, role);
   update(polyRamp, identifier.stageIndex);
   return result;
}
