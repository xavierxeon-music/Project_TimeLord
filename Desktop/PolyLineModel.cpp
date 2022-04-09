#include "PolyLineModel.h"

#include "MainWidget.h"

// items

PolyLine::Model::Items::Items(Model* model, const Data::Identifier& identifier)
{
   posItem = new QStandardItem();
   {
      posItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      posItem->setData(QVariant::fromValue(Data::Target::StageStartPosition), Data::Role::Target);
   }

   typeItem = new QStandardItem();
   {
      typeItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      typeItem->setData(QVariant::fromValue(Data::Target::StageType), Data::Role::Target);
   }

   endHeightItem = new QStandardItem();
   {
      endHeightItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      endHeightItem->setData(QVariant::fromValue(Data::Target::StageEndHeight), Data::Role::Target);
   }

   noteItem = new QStandardItem();
   {
      noteItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      noteItem->setEditable(false);
   }

   model->invisibleRootItem()->appendRow({posItem, typeItem, endHeightItem, noteItem});
}

// model

PolyLine::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Data::Core()
{
   setHorizontalHeaderLabels({"start position", "type", "end height", "note"});
}

void PolyLine::Model::rebuildModel(const Data::Identifier& identifier)
{
   clear();
   setHorizontalHeaderLabels({"start position", "type", "end height", "note"});

   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   if (0 == polyRamp->stageCount())
      return;

   {
      Items items(this, identifier);

      items.posItem->setText("start");
      items.posItem->setEditable(false);

      const Data::Type::Value type = Data::Type::Anchor;
      items.typeItem->setIcon(Data::Type::getIcon(type));
      items.typeItem->setText(Data::Type::getName(type));
      items.typeItem->setData(QVariant::fromValue(type), Data::Role::Data);
      items.typeItem->setEditable(false);

      const uint8_t startHeight = polyRamp->getStageStartHeight(0);

      items.endHeightItem->setText(QString::number(startHeight));

      const float voltage = static_cast<float>(startHeight * 5.0) / 255.0;
      const Note note = Note::fromVoltage(voltage);
      items.noteItem->setText(QString::fromStdString(note.name));
   }

   Data::Identifier stageIdentifier = identifier;

   uint32_t startPos = 0;
   for (uint8_t stageIndex = 0; stageIndex < polyRamp->stageCount(); stageIndex++)
   {
      const uint8_t nextIndex = (stageIndex + 1 < polyRamp->stageCount()) ? stageIndex + 1 : 0;
      const uint8_t startHeight = polyRamp->getStageStartHeight(stageIndex);
      const uint8_t endHeight = polyRamp->getStageStartHeight(nextIndex);
      const uint8_t length = polyRamp->getStageLength(stageIndex);

      stageIdentifier.stageIndex = stageIndex;
      Items items(this, stageIdentifier);

      Data::Type::Value type = Data::Type::Anchor;
      items.posItem->setText(QString::number(startPos));

      if (0 == length && 0 != nextIndex)
         type = Data::Type::Step;
      else if (startHeight < endHeight)
         type = Data::Type::Rise;
      else if (startHeight > endHeight)
         type = Data::Type::Fall;
      else
         type = Data::Type::Stable;

      items.typeItem->setIcon(Data::Type::getIcon(type));
      items.typeItem->setText(Data::Type::getName(type));
      items.typeItem->setData(QVariant::fromValue(type), Data::Role::Data);

      items.endHeightItem->setText(QString::number(endHeight));

      const float voltage = static_cast<float>(endHeight * 5.0) / 255.0;
      const Note note = Note::fromVoltage(voltage);
      items.noteItem->setText(QString::fromStdString(note.name));

      startPos += polyRamp->getStageLength(stageIndex);

      if (stageIndex + 1 == polyRamp->stageCount())
      {
         Items items(this, identifier);

         items.posItem->setText(QString::number(polyRamp->stageCount()));
         items.posItem->setEditable(false);

         const Data::Type::Value type = Data::Type::Anchor;
         items.typeItem->setIcon(Data::Type::getIcon(type));
         items.typeItem->setText(Data::Type::getName(type));
         items.typeItem->setData(QVariant::fromValue(type), Data::Role::Data);
         items.typeItem->setEditable(false);

         const uint8_t startHeight = polyRamp->getStageStartHeight(0);

         items.endHeightItem->setText(QString::number(startHeight));
         items.endHeightItem->setEditable(false);

         const float voltage = static_cast<float>(startHeight * 5.0) / 255.0;
         const Note note = Note::fromVoltage(voltage);
         items.noteItem->setText(QString::fromStdString(note.name));
      }
   }
}

bool PolyLine::Model::setData(const QModelIndex& index, const QVariant& value, int role)
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

   if (Data::Target::StageStartPosition == target)
   {
      const uint8_t startPos = value.toInt();
   }
   else if (Data::Target::StageType == target)
   {
      const Data::Type::Value type = value.value<Data::Type::Value>();
   }
   else if (Data::Target::StageEndHeight == target)
   {
      const uint8_t height = value.toInt();
   }

   bool result = QStandardItemModel::setData(index, targeValue, role);
   //update(polyRamp, identifier.stageIndex);
   return result;
}
