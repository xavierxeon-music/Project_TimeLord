#include "PolyLineModel.h"

#include <Music/Note.h>

#include "MainWidget.h"
#include "PolyLineList.h"

PolyLine::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Data::Core()
{
   setHorizontalHeaderLabels({"type", "start position", "start height", "end height"});
}

PolyLine::Model::Items PolyLine::Model::create(const Data::Identifier& identifier)
{
   Items items;

   items.typeItem = new QStandardItem();
   {
      items.typeItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      items.typeItem->setData(QVariant::fromValue(Data::Target::StageType), Data::Role::Target);
      items.typeItem->setEditable(false);
   }

   items.posItem = new QStandardItem();
   {
      items.posItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      items.posItem->setData(QVariant::fromValue(Data::Target::StageStartPosition), Data::Role::Target);
   }

   items.startHeigthItem = new QStandardItem();
   {
      items.startHeigthItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      items.startHeigthItem->setData(QVariant::fromValue(Data::Target::StageStartHeight), Data::Role::Target);
   }

   items.endHeightItem = new QStandardItem();
   {
      items.endHeightItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
      items.endHeightItem->setData(QVariant::fromValue(Data::Target::StageEndHeight), Data::Role::Target);
   }

   invisibleRootItem()->appendRow({items.typeItem, items.posItem, items.startHeigthItem, items.endHeightItem});

   return items;
}

PolyLine::Model::Items PolyLine::Model::find(const int& row)
{
   Items items;

   items.typeItem = invisibleRootItem()->child(row, 0);
   items.posItem = invisibleRootItem()->child(row, 1);
   items.startHeigthItem = invisibleRootItem()->child(row, 2);
   items.endHeightItem = invisibleRootItem()->child(row, 3);

   return items;
}

void PolyLine::Model::rebuildModel(Data::Identifier identifier)
{
   clear();
   setHorizontalHeaderLabels({"type", "start position", "start height", "end height"});

   PolyRamp* polyRamp = getPolyRamp(identifier);

   List list;
   list.compile(polyRamp);

   for (uint8_t index = 0; index < list.getStageCount(); index++)
   {
      Stage* stage = list.getStage(index);

      const bool isStartAchor = (Data::Type::StartAnchor == stage->type);
      const bool isEndAnchor = (Data::Type::EndAnchor == stage->type);
      const bool isAnchor = (isStartAchor || isEndAnchor);

      Data::Identifier stageIdentifier(identifier.rampIndex, isAnchor ? identifier.stageIndex : index - 1);

      Items items = create(stageIdentifier);

      items.typeItem->setIcon(Data::Type::getIcon(stage->type));
      items.typeItem->setText(Data::Type::getName(stage->type));
      items.typeItem->setData(QVariant::fromValue(stage->type), Data::Role::Data);

      if (isStartAchor)
         items.posItem->setText("start");
      else
         items.posItem->setText(QString::number(stage->startPosition));
      items.posItem->setEditable(!isAnchor);

      items.startHeigthItem->setText(QString::number(stage->startHeight));
      items.startHeigthItem->setEditable(!isEndAnchor);

      items.endHeightItem->setText(QString::number(stage->endHeight));
      items.endHeightItem->setEditable(!isEndAnchor);


      if (0 == index)
         continue;

      Stage* prevStage = list.getStage(index - 1);
      if (Data::Type::Step == prevStage->type)
      {
         items.posItem->setText("=");
         items.posItem->setEditable(false);
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
