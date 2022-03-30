#include "PolyLineModel.h"

#include "MainWidget.h"

// items

PolyLine::Model::Items::Items(Model* model, const Data::Identifier& identifier)
{
   startPosItem = new QStandardItem();
   {
      startPosItem->setData(QVariant::fromValue(identifier.provider), Data::Role::Provider);
      startPosItem->setData(QVariant::fromValue(identifier.rampIndex), Data::Role::GraphIndex);
      startPosItem->setData(QVariant::fromValue(identifier.stageIndex), Data::Role::StageIndex);
      startPosItem->setEditable(false);
   }

   typeItem = new QStandardItem();
   {
      typeItem->setData(QVariant::fromValue(identifier.provider), Data::Role::Provider);
      typeItem->setData(QVariant::fromValue(identifier.rampIndex), Data::Role::GraphIndex);
      typeItem->setData(QVariant::fromValue(identifier.stageIndex), Data::Role::StageIndex);
      typeItem->setData(QVariant::fromValue(Data::Target::StageType), Data::Role::Target);
   }

   endHeightItem = new QStandardItem();
   {
      endHeightItem->setData(QVariant::fromValue(identifier.provider), Data::Role::Provider);
      endHeightItem->setData(QVariant::fromValue(identifier.rampIndex), Data::Role::GraphIndex);
      endHeightItem->setData(QVariant::fromValue(identifier.stageIndex), Data::Role::StageIndex);
      endHeightItem->setData(QVariant::fromValue(Data::Target::StageEndHeight), Data::Role::Target);
   }

   noteItem = new QStandardItem();
   {
      noteItem->setData(QVariant::fromValue(identifier.provider), Data::Role::Provider);
      noteItem->setData(QVariant::fromValue(identifier.rampIndex), Data::Role::GraphIndex);
      noteItem->setData(QVariant::fromValue(identifier.stageIndex), Data::Role::StageIndex);
      noteItem->setEditable(false);
   }

   model->invisibleRootItem()->appendRow({startPosItem, typeItem, endHeightItem, noteItem});
}
// model

PolyLine::Model::Model(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , Data::Core(mainWidget)
{
   setHorizontalHeaderLabels({"start position", "type", "end height", "note"});
}

void PolyLine::Model::rebuild(const Data::Identifier& identifier)
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

      items.startPosItem->setText("start");

      items.typeItem->setIcon(QIcon(":/TrendAnchor.svg"));
      items.typeItem->setText("anchor");
      items.typeItem->setData(QVariant::fromValue(Data::Type::Step), Data::Role::Data);

      const uint8_t startHeight = polyRamp->getStageStartHeight(0);

      items.endHeightItem->setText(QString::number(startHeight));

      const float voltage = static_cast<float>(startHeight * 5.0) / 255.0;
      const Note note = Note::fromVoltage(voltage);
      items.noteItem->setText(QString::fromStdString(note.name));
   }

   uint32_t startPos = 0;
   for (uint8_t stageIndex = 0; stageIndex < polyRamp->stageCount(); stageIndex++)
   {
      const uint8_t nextIndex = (stageIndex + 1 < polyRamp->stageCount()) ? stageIndex + 1 : 0;
      const uint8_t startHeight = polyRamp->getStageStartHeight(stageIndex);
      const uint8_t endHeight = polyRamp->getStageStartHeight(nextIndex);
      const uint8_t length = polyRamp->getStageLength(stageIndex);

      Items items(this, identifier);

      items.startPosItem->setText(QString::number(startPos));

      if (0 == length && 0 != nextIndex)
      {
         items.typeItem->setIcon(QIcon(":/TrendStep.svg"));
         items.typeItem->setText("step");
         items.typeItem->setData(QVariant::fromValue(Data::Type::Step), Data::Role::Data);
      }
      else if (startHeight < endHeight)
      {
         items.typeItem->setIcon(QIcon(":/TrendRise.svg"));
         items.typeItem->setText("rise");
         items.typeItem->setData(QVariant::fromValue(Data::Type::Rise), Data::Role::Data);
      }
      else if (startHeight > endHeight)
      {
         items.typeItem->setIcon(QIcon(":/TrendFall.svg"));
         items.typeItem->setText("fall");
         items.typeItem->setData(QVariant::fromValue(Data::Type::Fall), Data::Role::Data);
      }
      else
      {
         items.typeItem->setIcon(QIcon(":/TrendStable.svg"));
         items.typeItem->setText("stable");
         items.typeItem->setData(QVariant::fromValue(Data::Type::Stable), Data::Role::Data);
      }

      items.endHeightItem->setText(QString::number(endHeight));

      const float voltage = static_cast<float>(endHeight * 5.0) / 255.0;
      const Note note = Note::fromVoltage(voltage);
      items.noteItem->setText(QString::fromStdString(note.name));

      startPos += polyRamp->getStageLength(stageIndex);

      if (stageIndex + 1 == polyRamp->stageCount())
      {
         Items items(this, identifier);

         items.startPosItem->setText("end");

         items.typeItem->setIcon(QIcon(":/TrendAnchor.svg"));
         items.typeItem->setText("anchor");
         items.typeItem->setData(QVariant::fromValue(Data::Type::Step), Data::Role::Data);

         const uint8_t startHeight = polyRamp->getStageStartHeight(0);

         items.endHeightItem->setText(QString::number(startHeight));
         items.endHeightItem->setEditable(false);

         const float voltage = static_cast<float>(startHeight * 5.0) / 255.0;
         const Note note = Note::fromVoltage(voltage);
         items.noteItem->setText(QString::fromStdString(note.name));
      }
   }
}
