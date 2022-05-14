#include "RampModel.h"

#include <FileSettings.h>

#include "MainWidget.h"
#include "ModelDivision.h"

Ramp::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Core::Interface()
{
   setHorizontalHeaderLabels({"name", "length", "division", "loop", "count"});
}

void Ramp::Model::modelHasChanged(Core::Identifier identifier)
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* nameItem = invisibleRootItem()->child(row, 0);
      const Core::Identifier itemIndentifier = nameItem->data(Core::Role::Identifier).value<Core::Identifier>();
      if (itemIndentifier.rampIndex != identifier.rampIndex)
         continue;

      PolyRamp* polyRamp = getPolyRamp(identifier);

      QStandardItem* lengthItem = invisibleRootItem()->child(row, 1);
      const QString length = QString::number(polyRamp->getLength());
      lengthItem->setText(length);

      QStandardItem* countItem = invisibleRootItem()->child(row, 4);
      const QString count = QString::number(polyRamp->getStageCount());
      countItem->setText(count);

      break;
   }
}

void Ramp::Model::rebuildModel(Core::Identifier)
{
   clear();
   setHorizontalHeaderLabels({"name", "length", "division", "loop", "count"});

   FileSettings settings("names");

   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
   {
      Core::Identifier identifier(rampIndex);
      PolyRamp* polyRamp = getPolyRamp(identifier);
      if (!polyRamp)
         continue;

      QStandardItem* nameItem = new QStandardItem();
      {
         QString name = QString::number(rampIndex + 1);
         if (1 == name.length())
            name = QString("0") + name;

         const QString key = keys.at(identifier.rampIndex);
         name = settings.string(key, name);

         nameItem->setText(name);
         nameItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         nameItem->setData(QVariant::fromValue(Core::Target::PolyRampName), Core::Role::Target);
      }

      QStandardItem* lengthItem = new QStandardItem();
      {
         const QString length = QString::number(polyRamp->getLength());
         lengthItem->setText(length);
         lengthItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         lengthItem->setData(QVariant::fromValue(Core::Target::PolyRampLength), Core::Role::Target);
      }

      QStandardItem* stepSizeItem = new QStandardItem();
      {
         const std::string stepSize = Tempo::getName(polyRamp->getStepSize());
         stepSizeItem->setText(QString::fromStdString(stepSize));
         stepSizeItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         stepSizeItem->setData(QVariant::fromValue(polyRamp->getStepSize()), Core::Role::Data);
         stepSizeItem->setData(QVariant::fromValue(Core::Target::PolyRampStepSize), Core::Role::Target);
      }

      QStandardItem* loopItem = new QStandardItem();
      {
         loopItem->setCheckable(true);
         loopItem->setCheckState(polyRamp->isLooping() ? Qt::Checked : Qt::Unchecked);
         loopItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         loopItem->setData(QVariant::fromValue(Core::Target::PolyRampLoop), Core::Role::Target);
      }

      QStandardItem* countItem = new QStandardItem();
      {
         const QString count = QString::number(polyRamp->getStageCount());
         countItem->setText(count);
         countItem->setData(QVariant::fromValue(identifier), Core::Role::Identifier);
         countItem->setEditable(false);
      }

      invisibleRootItem()->appendRow({nameItem, lengthItem, stepSizeItem, loopItem, countItem});
   }
}

void Ramp::Model::saveSettings()
{
   FileSettings settings("names");
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* nameItem = invisibleRootItem()->child(row, 0);
      const QString name = nameItem->text();

      const Core::Identifier itemIndentifier = nameItem->data(Core::Role::Identifier).value<Core::Identifier>();
      const QString key = keys.at(itemIndentifier.rampIndex);

      settings.write(key, name);
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
      setModified();
   }
   else if (Core::Target::PolyRampLength == target)
   {
      const uint8_t length = value.toInt();
      polyRamp->setLength(length);
      callOnAllInstances(&Interface::modelHasChanged, identifier);
   }
   else if (Core::Target::PolyRampStepSize == target)
   {
      const Tempo::Division stepSize = value.value<Tempo::Division>();
      const std::string name = Tempo::getName(stepSize);
      result = QStandardItemModel::setData(index, QString::fromStdString(name), Qt::EditRole);

      polyRamp->setStepSize(stepSize);

      callOnAllInstances(&Interface::modelHasChanged, identifier);
   }
   else if (Core::Target::PolyRampLoop == target)
   {
      bool loopOn = value.toBool();
      polyRamp->setLooping(loopOn);
   }

   return result;
}
