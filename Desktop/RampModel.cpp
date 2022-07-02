#include "RampModel.h"

#include <FileSettings.h>

#include "MainWidget.h"
#include "ModelDivision.h"

Ramp::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Core::Interface()
{
   setHorizontalHeaderLabels({"name", "length", "division", "time", "loop", "count"});
}

Ramp::Model::Items Ramp::Model::create(const Core::Identifier& identifier)
{
   Items items;

   items.nameItem = new QStandardItem();
   {
      items.nameItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.nameItem->setData(QVariant::fromValue(Core::Target::PolyRampName), Core::Role::Target);
   }

   items.lengthItem = new QStandardItem();
   {
      items.lengthItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.lengthItem->setData(QVariant::fromValue(Core::Target::PolyRampLength), Core::Role::Target);
   }

   items.stepSizeItem = new QStandardItem();
   {
      items.stepSizeItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.stepSizeItem->setData(QVariant::fromValue(Core::Target::PolyRampStepSize), Core::Role::Target);
   }

   items.timeItem = new QStandardItem();
   {
      items.timeItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.timeItem->setData(QVariant::fromValue(Core::Target::PolyRampTime), Core::Role::Target);
      items.timeItem->setEditable(false);
   }

   items.loopItem = new QStandardItem();
   {
      items.loopItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.loopItem->setData(QVariant::fromValue(Core::Target::PolyRampLoop), Core::Role::Target);
      items.loopItem->setCheckable(true);
      items.loopItem->setEditable(false);
   }

   items.countItem = new QStandardItem();
   {
      items.countItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
      items.countItem->setEditable(false);
   }

   invisibleRootItem()->appendRow({items.nameItem, items.lengthItem, items.stepSizeItem, items.timeItem, items.loopItem, items.countItem});

   return items;
}

Ramp::Model::Items Ramp::Model::find(const int& row)
{
   Items items;

   items.nameItem = invisibleRootItem()->child(row, 0);
   items.lengthItem = invisibleRootItem()->child(row, 1);
   items.stepSizeItem = invisibleRootItem()->child(row, 2);
   items.timeItem = invisibleRootItem()->child(row, 3);
   items.loopItem = invisibleRootItem()->child(row, 4);
   items.countItem = invisibleRootItem()->child(row, 5);

   return items;
}

void Ramp::Model::rebuildModel(Core::Identifier identifier)
{
   clear();
   setHorizontalHeaderLabels({"name", "length", "division", "time", "loop", "count"});

   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
   {
      identifier.rampIndex = rampIndex;

      PolyRamp* polyRamp = getPolyRamp(identifier);
      if (!polyRamp)
         continue;

      create(identifier);
      modelHasChanged(identifier);
   }
}

void Ramp::Model::modelHasChanged(Core::Identifier identifier)
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      Items items = find(row);
      const Core::Identifier itemIndentifier = items.nameItem->data(Core::Role::Identifier).value<Core::Identifier>();
      if (itemIndentifier.rampIndex != identifier.rampIndex)
         continue;

      Bank::Content* bank = getBank(identifier);
      PolyRamp* polyRamp = getPolyRamp(identifier);

      {
         const QString name = bank->getName(identifier.rampIndex);
         items.nameItem->setText(name);
      }

      {
         const QString length = QString::number(polyRamp->getLength());
         items.lengthItem->setText(length);
      }

      {
         const std::string stepSize = Tempo::getName(polyRamp->getStepSize());
         items.stepSizeItem->setText(QString::fromStdString(stepSize));
         items.stepSizeItem->setData(QVariant::fromValue(polyRamp->getStepSize()), Core::Role::Data);
      }

      {
         const Tempo::Division division = items.stepSizeItem->data(Core::Role::Data).value<Tempo::Division>();
         const uint32_t divisionCount = items.lengthItem->text().toInt();

         const QString timeText = compileTime(bank, division, divisionCount);
         items.timeItem->setText(timeText);
      }

      {
         items.loopItem->setCheckState(polyRamp->isLooping() ? Qt::Checked : Qt::Unchecked);
      }

      {
         const QString count = QString::number(polyRamp->getStageCount());
         items.countItem->setText(count);
      }
      break;
   }
}

bool Ramp::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   bool result = QStandardItemModel::setData(index, value, role);
   if (Qt::EditRole != role && Qt::CheckStateRole != role)
      return result;

   const QVariant targetData = data(index, Core::Role::Target);
   if (targetData.isNull())
      return result;

   const Core::Identifier identifier = data(index, Core::Role::Identifier).value<Core::Identifier>();
   PolyRamp* polyRamp = getPolyRamp(identifier);

   const Core::Target::Value target = targetData.value<Core::Target::Value>();

   if (Core::Target::PolyRampName == target)
   {
      Bank::Content* bank = getBank(identifier);
      bank->setName(identifier.rampIndex, value.toString());
      setModified();
   }
   else if (Core::Target::PolyRampLength == target)
   {
      const uint32_t length = value.toInt();
      polyRamp->setLength(length);

      callOnAllInstances(&Interface::modelHasChanged, identifier);
      modelHasChanged(identifier);
      setModified();
   }
   else if (Core::Target::PolyRampStepSize == target)
   {
      const Tempo::Division stepSize = value.value<Tempo::Division>();
      const std::string name = Tempo::getName(stepSize);
      result = QStandardItemModel::setData(index, QString::fromStdString(name), Qt::EditRole);

      polyRamp->setStepSize(stepSize);

      callOnAllInstances(&Interface::modelHasChanged, identifier);
      modelHasChanged(identifier);
      setModified();
   }
   else if (Core::Target::PolyRampLoop == target)
   {
      bool loopOn = value.toBool();
      polyRamp->setLooping(loopOn);

      setModified();
   }

   return result;
}
