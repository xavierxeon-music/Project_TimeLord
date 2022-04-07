#include "RampModel.h"

#include "MainWidget.h"

Ramp::Model::Model(QObject* parent)
   : QStandardItemModel(parent)
   , Data::Core()
{
   setHorizontalHeaderLabels({"name", "length", "division", "loop", "count"});
}

void Ramp::Model::slotRampChanged(const Data::Identifier& identifier)
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* nameItem = invisibleRootItem()->child(row, 0);
      const Data::Identifier itemIndentifier = nameItem->data(Data::Role::Identifier).value<Data::Identifier>();
      if (itemIndentifier.rampIndex != identifier.rampIndex)
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

   for (uint8_t rampIndex = 0; rampIndex < 16; rampIndex++)
   {
      Data::Identifier identifier(rampIndex);
      PolyRamp* polyRamp = getPolyRamp(identifier);

      QStandardItem* nameItem = new QStandardItem();
      {
         QString name = QString::number(rampIndex + 1);
         if (1 == name.length())
            name = QString("0") + name;

         nameItem->setText(name);
         nameItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
         nameItem->setEditable(false);
      }

      QStandardItem* lengthItem = new QStandardItem();
      {
         const QString length = QString::number(polyRamp->getLength());
         lengthItem->setText(length);
         lengthItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
         lengthItem->setData(QVariant::fromValue(Data::Target::GraphLength), Data::Role::Target);
      }

      QStandardItem* stepSizeItem = new QStandardItem();
      {
         const std::string stepSize = Tempo::getName(polyRamp->getStepSize());
         stepSizeItem->setText(QString::fromStdString(stepSize));
         stepSizeItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
         stepSizeItem->setData(QVariant::fromValue(polyRamp->getStepSize()), Data::Role::Data);
         stepSizeItem->setData(QVariant::fromValue(Data::Target::GraphStepSize), Data::Role::Target);
      }

      QStandardItem* loopItem = new QStandardItem();
      {
         loopItem->setCheckable(true);
         loopItem->setCheckState(polyRamp->isLooping() ? Qt::Checked : Qt::Unchecked);
         loopItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
         loopItem->setData(QVariant::fromValue(Data::Target::GraphLoop), Data::Role::Target);
      }

      QStandardItem* countItem = new QStandardItem();
      {
         const QString count = QString::number(polyRamp->stageCount());
         countItem->setText(count);
         countItem->setData(QVariant::fromValue(identifier), Data::Role::Identifier);
         countItem->setEditable(false);
      }

      invisibleRootItem()->appendRow({nameItem, lengthItem, stepSizeItem, loopItem, countItem});
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

   const Data::Identifier identifier = data(index, Data::Role::Identifier).value<Data::Identifier>();
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
