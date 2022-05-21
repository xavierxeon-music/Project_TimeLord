#include "StageModel.h"

#include "MainWidget.h"

Stage::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Core::Interface()
{
   setHorizontalHeaderLabels({"start position", "start time", "length", "start height", "end height"});
}

Stage::Model::Items Stage::Model::create(const Core::Identifier& identifier)
{
   Items items;

   items.startPosItem = new QStandardItem();
   {
      items.startPosItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.startPosItem->setEditable(false);
   }

   items.startTimeItem = new QStandardItem();
   {
      items.startTimeItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.startTimeItem->setEditable(false);
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

   invisibleRootItem()->appendRow({items.startPosItem, items.startTimeItem, items.lengthItem, items.startHeigthItem, items.endHeigthItem});

   return items;
}

Stage::Model::Items Stage::Model::find(const int& row)
{
   Items items;

   items.startPosItem = invisibleRootItem()->child(row, 0);
   items.startTimeItem = invisibleRootItem()->child(row, 1);
   items.lengthItem = invisibleRootItem()->child(row, 2);
   items.startHeigthItem = invisibleRootItem()->child(row, 3);
   items.endHeigthItem = invisibleRootItem()->child(row, 4);

   return items;
}

void Stage::Model::rebuildModel(Core::Identifier identifier)
{
   clear();
   setHorizontalHeaderLabels({"start position", "start time", "length", "start height", "end height"});

   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   Core::Identifier stageIdentifier = identifier;

   for (uint8_t stageIndex = 0; stageIndex < polyRamp->getStageCount(); stageIndex++)
   {
      stageIdentifier.stageIndex = stageIndex;
      create(stageIdentifier);
   }

   modelHasChanged(identifier);
   callOnAllInstances(&Interface::modelHasChanged, identifier);
}

void Stage::Model::modelHasChanged(Core::Identifier ignoreIdentifier)
{
   Q_UNUSED(ignoreIdentifier)

   uint32_t startPos = 0;
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      Items items = find(row);
      const Core::Identifier itemIndentifier = items.startPosItem->data(Core::Role::Identifier).value<Core::Identifier>();

      Bank::Content* bank = getBank(itemIndentifier);
      PolyRamp* polyRamp = getPolyRamp(itemIndentifier);

      const uint8_t startHeight = polyRamp->getStageStartHeight(itemIndentifier.stageIndex);
      const uint8_t endHeight = polyRamp->getStageEndHeight(itemIndentifier.stageIndex);
      const uint8_t length = polyRamp->getStageLength(itemIndentifier.stageIndex);

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

      //if (identifier.stageIndex == itemStageIndex)
      {
         QString lengthText = QString::number(length);
         if (itemIndentifier.stageIndex + 1 == polyRamp->getStageCount())
            lengthText += "*";
         items.lengthItem->setText(lengthText);

         const QString startHeightText = QString::number(startHeight);
         items.startHeigthItem->setText(startHeightText);

         const QString endHeightText = QString::number(endHeight);
         items.endHeigthItem->setText(endHeightText);
      }

      const Tempo::Division division = polyRamp->getStepSize();
      const QString timeText = compileTime(bank, division, startPos);
      items.startTimeItem->setText(timeText);

      startPos += length;
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

      modelHasChanged(identifier);
      setModified();
   }
   else if (Core::Target::StageEndHeight == target)
   {
      const uint8_t height = value.toInt();
      polyRamp->setStageEndHeight(identifier.stageIndex, height);
      callOnAllInstances(&Interface::modelHasChanged, identifier);

      modelHasChanged(identifier);
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

         modelHasChanged(identifier);
         setModified();
      }
   }

   bool result = QStandardItemModel::setData(index, targeValue, role);
   return result;
}
