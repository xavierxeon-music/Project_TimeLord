#include "StageModel.h"

#include "MainWidget.h"

// items

Stage::Model::Items::Items(Model* model, const Data::Identifier& identifier)
{
   indexItem = new QStandardItem();
   {
      QString name = QString::number(identifier.stageIndex + 1);
      while (3 != name.length())
         name = QString("0") + name;

      indexItem->setText(name);
      indexItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      indexItem->setEditable(false);
   }

   startPosItem = new QStandardItem();
   {
      startPosItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      startPosItem->setEditable(false);
   }

   lengthItem = new QStandardItem();
   {
      lengthItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      lengthItem->setData(QVariant::fromValue(Data::Target::StageLength), Data::Role::Target);
   }

   startHeigthItem = new QStandardItem();
   {
      startHeigthItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      startHeigthItem->setData(QVariant::fromValue(Data::Target::StageStartHeight), Data::Role::Target);
   }

   model->invisibleRootItem()->appendRow({indexItem, startPosItem, lengthItem, startHeigthItem});
}

Stage::Model::Items::Items(Model* model, const int& row)
{
   indexItem = model->invisibleRootItem()->child(row, 0);
   startPosItem = model->invisibleRootItem()->child(row, 1);
   lengthItem = model->invisibleRootItem()->child(row, 2);
   startHeigthItem = model->invisibleRootItem()->child(row, 3);
}

// model

Stage::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Data::Core()

{
   setHorizontalHeaderLabels({"index", "start position", "length", "start height"});
}

void Stage::Model::rebuild(const Data::Identifier& identifier)
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
      Items items(this, stageIdentifier);

      QString name = QString::number(stageIndex + 1);
      while (3 != name.length())
         name = QString("0") + name;

      items.indexItem->setText(name);

      update(polyRamp, stageIndex);
   }

   emit signalRampChanged(identifier);
}

void Stage::Model::update(PolyRamp* polyRamp, const uint8_t& itemStageIndex)
{
   uint32_t startPos = 0;
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      Items items(this, row);

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

   const Data::Target target = targetData.value<Data::Target>();
   if (Data::Target::StageStartHeight == target)
   {
      const uint8_t height = value.toInt();
      polyRamp->setStageStartHeight(identifier.stageIndex, height);
      emit signalRampChanged(identifier);
   }
   else if (Data::Target::StageLength == target)
   {
      QString length = value.toString().replace("*", "");
      if (PolyRamp::LengthStatus::Changed != polyRamp->setStageLength(identifier.stageIndex, length.toInt(), !lockGraphSize))
      {
         targeValue = polyRamp->getStageLength(identifier.stageIndex); // UNDO
      }
      else
      {
         if (identifier.stageIndex + 1 == polyRamp->stageCount())
            length += "*";
         targeValue = length;
         emit signalRampChanged(identifier);
      }
   }

   bool result = QStandardItemModel::setData(index, targeValue, role);
   update(polyRamp, identifier.stageIndex);
   return result;
}
