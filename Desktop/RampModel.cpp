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

void Ramp::Model::modelHasChanged(Core::Identifier identifier)
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* nameItem = invisibleRootItem()->child(row, 0);
      const Core::Identifier itemIndentifier = nameItem->data(Core::Role::Identifier).value<Core::Identifier>();
      if (itemIndentifier.rampIndex != identifier.rampIndex)
         continue;

      Bank::Content* bank = getBank(identifier);
      PolyRamp* polyRamp = getPolyRamp(identifier);

      {
         const QString name = bank->getName(identifier.rampIndex);
         nameItem->setText(name);
      }

      QStandardItem* lengthItem = invisibleRootItem()->child(row, 1);
      {
         const QString length = QString::number(polyRamp->getLength());
         lengthItem->setText(length);
      }

      QStandardItem* stepSizeItem = invisibleRootItem()->child(row, 2);
      {
         const std::string stepSize = Tempo::getName(polyRamp->getStepSize());
         stepSizeItem->setText(QString::fromStdString(stepSize));
         stepSizeItem->setData(QVariant::fromValue(polyRamp->getStepSize()), Core::Role::Data);
      }

      QStandardItem* timeItem = invisibleRootItem()->child(row, 3);
      {
         const float beatsPerMinute = bank->getBeatsPerMinute(); // == quarter / min
         const float sixteenthPerSecond = (4 * beatsPerMinute) / 60.0;

         const Tempo::Division division = stepSizeItem->data(Core::Role::Data).value<Tempo::Division>();
         const float sixteenthPerDivision = static_cast<float>(division);

         const float secondsPerDivision = sixteenthPerDivision / sixteenthPerSecond;
         const float divisionCount = lengthItem->text().toInt();

         const uint32_t time = static_cast<uint32_t>(divisionCount * secondsPerDivision);
         const uint8_t seconds = time % 60;
         const uint32_t minutes = (time - seconds) / 60;

         const QString timeText = QString("%1:%2").arg(minutes).arg(seconds);
         timeItem->setText(timeText);
      }

      QStandardItem* loopItem = invisibleRootItem()->child(row, 4);
      {
         loopItem->setCheckState(polyRamp->isLooping() ? Qt::Checked : Qt::Unchecked);
      }

      QStandardItem* countItem = invisibleRootItem()->child(row, 5);
      {
         const QString count = QString::number(polyRamp->getStageCount());
         countItem->setText(count);
      }
      break;
   }
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

      QStandardItem* nameItem = new QStandardItem();
      {
         nameItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         nameItem->setData(QVariant::fromValue(Core::Target::PolyRampName), Core::Role::Target);
      }

      QStandardItem* lengthItem = new QStandardItem();
      {
         lengthItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         lengthItem->setData(QVariant::fromValue(Core::Target::PolyRampLength), Core::Role::Target);
      }

      QStandardItem* stepSizeItem = new QStandardItem();
      {
         stepSizeItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         stepSizeItem->setData(QVariant::fromValue(Core::Target::PolyRampStepSize), Core::Role::Target);
      }

      QStandardItem* timeItem = new QStandardItem();
      {
         timeItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         timeItem->setData(QVariant::fromValue(Core::Target::PolyRampTime), Core::Role::Target);
         timeItem->setEditable(false);
      }

      QStandardItem* loopItem = new QStandardItem();
      {
         loopItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         loopItem->setData(QVariant::fromValue(Core::Target::PolyRampLoop), Core::Role::Target);
         loopItem->setCheckable(true);
         loopItem->setEditable(false);
      }

      QStandardItem* countItem = new QStandardItem();
      {
         countItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         countItem->setEditable(false);
      }

      invisibleRootItem()->appendRow({nameItem, lengthItem, stepSizeItem, timeItem, loopItem, countItem});

      modelHasChanged(identifier);
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
      const uint8_t length = value.toInt();
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
