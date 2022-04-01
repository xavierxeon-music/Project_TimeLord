#include "RampModel.h"

#include "MainWidget.h"

Ramp::Model::Model(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , Data::Core(mainWidget)
{
   setHorizontalHeaderLabels({"name", "length", "division", "loop", "count"});
}

void Ramp::Model::slotRampChanged(const Data::Identifier& identifier)
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* nameItem = invisibleRootItem()->child(row, 0);
      if (nameItem->data(Data::Role::Device).value<Data::RampDevice>() != identifier.device)
         continue;
      if (nameItem->data(Data::Role::RampIndex).value<uint8_t>() != identifier.rampIndex)
         continue;

      PolyRamp* polyRamp = getPolyRamp(identifier);

      QStandardItem* lengthItem = invisibleRootItem()->child(row, 1);
      const QString length = QString::number(polyRamp->getLength());
      lengthItem->setText(length);

      QStandardItem* countItem = invisibleRootItem()->child(row, 4);
      const QString count = QString::number(polyRamp->stageCount());
      countItem->setText(count);

      break;
   }
}

void Ramp::Model::rebuild()
{
   clear();
   setHorizontalHeaderLabels({"name", "length", "division", "loop", "count"});

   const DeviceNameMap& nameMap = getDeviceNameMap();
   for (DeviceNameMap::const_iterator it = nameMap.constBegin(); it != nameMap.constEnd(); it++)
   {
      const Data::RampDevice provider = it.key();

      for (uint8_t rampIndex = 0; rampIndex < 16; rampIndex++)
      {
         Data::Identifier identifier(provider, rampIndex);
         PolyRamp* polyRamp = getPolyRamp(identifier);

         QStandardItem* nameItem = new QStandardItem();
         {
            QString name = QString::number(rampIndex + 1);
            if (1 == name.length())
               name = QString("0") + name;

            nameItem->setText(it.value() + " " + name);
            nameItem->setData(QVariant::fromValue(provider), Data::Role::Device);
            nameItem->setData(QVariant::fromValue(rampIndex), Data::Role::RampIndex);
            nameItem->setEditable(false);
         }

         QStandardItem* lengthItem = new QStandardItem();
         {
            const QString length = QString::number(polyRamp->getLength());
            lengthItem->setText(length);
            lengthItem->setData(QVariant::fromValue(provider), Data::Role::Device);
            lengthItem->setData(QVariant::fromValue(rampIndex), Data::Role::RampIndex);
            lengthItem->setData(QVariant::fromValue(Data::Target::GraphLength), Data::Role::Target);
         }

         QStandardItem* stepSizeItem = new QStandardItem();
         {
            const std::string stepSize = Tempo::getName(polyRamp->getStepSize());
            stepSizeItem->setText(QString::fromStdString(stepSize));
            stepSizeItem->setData(QVariant::fromValue(provider), Data::Role::Device);
            stepSizeItem->setData(QVariant::fromValue(rampIndex), Data::Role::RampIndex);
            stepSizeItem->setData(QVariant::fromValue(polyRamp->getStepSize()), Data::Role::Data);
            stepSizeItem->setData(QVariant::fromValue(Data::Target::GraphStepSize), Data::Role::Target);
         }

         QStandardItem* loopItem = new QStandardItem();
         {
            loopItem->setCheckable(true);
            loopItem->setCheckState(polyRamp->isLooping() ? Qt::Checked : Qt::Unchecked);
            loopItem->setData(QVariant::fromValue(provider), Data::Role::Device);
            loopItem->setData(QVariant::fromValue(rampIndex), Data::Role::RampIndex);
            loopItem->setData(QVariant::fromValue(Data::Target::GraphLoop), Data::Role::Target);
         }

         QStandardItem* countItem = new QStandardItem();
         {
            const QString count = QString::number(polyRamp->stageCount());
            countItem->setText(count);
            countItem->setData(QVariant::fromValue(provider), Data::Role::Device);
            countItem->setData(QVariant::fromValue(rampIndex), Data::Role::RampIndex);
            countItem->setEditable(false);
         }

         invisibleRootItem()->appendRow({nameItem, lengthItem, stepSizeItem, loopItem, countItem});
      }
   }
}

bool Ramp::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   bool result = QStandardItemModel::setData(index, value, role);
   if (Qt::EditRole != role && Qt::CheckStateRole != role)
      return result;

   const QVariant targetData = data(index, Data::Role::Target);
   if (targetData.isNull())
      return result;

   const Data::RampDevice provider = data(index, Data::Role::Device).value<Data::RampDevice>();
   const uint8_t rampIndex = data(index, Data::Role::RampIndex).value<uint8_t>();

   Data::Identifier identifier(provider, rampIndex);
   PolyRamp* polyRamp = getPolyRamp(identifier);

   const Data::Target target = targetData.value<Data::Target>();
   if (Data::Target::GraphLength == target)
   {
      const uint8_t length = value.toInt();
      polyRamp->setLength(length);
   }
   else if (Data::Target::GraphStepSize == target)
   {
      const Tempo::Division stepSize = value.value<Tempo::Division>();
      const std::string name = Tempo::getName(stepSize);
      result = QStandardItemModel::setData(index, QString::fromStdString(name), Qt::EditRole);

      polyRamp->setStepSize(stepSize);
   }
   else if (Data::Target::GraphLoop == target)
   {
      bool loopOn = value.toBool();
      polyRamp->setLooping(loopOn);
   }

   return result;
}
