#include "PolyLineModel.h"

#include "MainWidget.h"
#include "PolyLineList.h"

// items

PolyLine::Model::Items::Items(Model* model, const Data::Identifier& identifier)
{
   typeItem = new QStandardItem();
   {
      typeItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      typeItem->setData(QVariant::fromValue(Data::Target::StageType), Data::Role::Target);
      typeItem->setEditable(false);
   }

   posItem = new QStandardItem();
   {
      posItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      posItem->setData(QVariant::fromValue(Data::Target::StageStartPosition), Data::Role::Target);
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

   model->invisibleRootItem()->appendRow({typeItem, posItem, endHeightItem, noteItem});
}

// model

PolyLine::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Data::Core()
{
   setHorizontalHeaderLabels({"type", "start position", "end height", "note"});
}

void PolyLine::Model::rebuildModel(const Data::Identifier& identifier)
{
   clear();
   setHorizontalHeaderLabels({"type", "start position", "end height", "note"});

   PolyRamp* polyRamp = getPolyRamp(identifier);

   List list;
   list.compile(polyRamp);

   for (uint8_t index = 0; index < list.stageCount(); index++)
   {
      Stage* stage = list.getStage(index);

      const bool isStartAchor = (Data::Type::StartAnchor == stage->type);
      const bool isEndAnchor = (Data::Type::EndAnchor == stage->type);
      const bool isAnchor = (isStartAchor || isEndAnchor);

      Data::Identifier stageIdentifier(identifier.rampIndex, isAnchor ? identifier.stageIndex : index - 1);

      Items items(this, stageIdentifier);

      items.typeItem->setIcon(Data::Type::getIcon(stage->type));
      items.typeItem->setText(Data::Type::getName(stage->type));
      items.typeItem->setData(QVariant::fromValue(stage->type), Data::Role::Data);

      if (isStartAchor)
         items.posItem->setText("start");
      else
         items.posItem->setText(QString::number(stage->startPosition));
      items.posItem->setEditable(!isAnchor);

      items.endHeightItem->setText(QString::number(stage->endHeight));
      items.endHeightItem->setEditable(!isEndAnchor);

      const float voltage = static_cast<float>(stage->endHeight * 5.0) / 255.0;
      const Note note = Note::fromVoltage(voltage);
      items.noteItem->setText(QString::fromStdString(note.name));
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
