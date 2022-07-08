#include "DeviceStateModel.h"

#include "Target.h"

DeviceState::Model::Model(QObject* parent, Target* target)
   : QStandardItemModel(parent)
   , Core::Interface()
   , currentStage()
   , stageList()
   , bank(nullptr)

{
   connect(target, &Target::signalNewState, this, &Model::slotNewState);

   setHorizontalHeaderLabels({"number", "start pos", "time"});

   Core::Identifier dummy;
   bank = getBank(dummy);
}

void DeviceState::Model::applyToBanks()
{
   for (const Stage& stage : stageList)
   {
      qDebug() << stage.position << stage.map;
   }
}

QString DeviceState::Model::compileInfo() const
{
   QString info;
   info += QString::number(getBankCount()) + " banks";

   info += " @ " + QString::number(bank->getBeatsPerMinute()) + " bpm";

   return info;
}

void DeviceState::Model::slotNewState(const QJsonObject& stateObject)
{
   uint8_t bankIndex = stateObject["bankIndex"].toInt();
   if (bankIndex < 0 || bankIndex >= getBankCount())
      return;

   currentStage.map[bankIndex] = stateObject;
   if (currentStage.map.size() < getBankCount())
      return;

   stageList.append(currentStage);

   const uint16_t index = invisibleRootItem()->rowCount();

   QStandardItem* numberItem = new QStandardItem();
   numberItem->setText(QString::number(index));
   numberItem->setEditable(false);

   QStandardItem* posItem = new QStandardItem();
   posItem->setText(QString::number(index));

   const uint32_t position = index;
   const QString timeText = compileTime(bank, Tempo::Bar, position);

   QStandardItem* timeItem = new QStandardItem();
   timeItem->setText(timeText);
   timeItem->setEditable(false);

   invisibleRootItem()->appendRow({numberItem, posItem, timeItem});
   currentStage.map.clear();
}

bool DeviceState::Model::setData(const QModelIndex& index, const QVariant& value, int role)
{
   bool result = QStandardItemModel::setData(index, value, role);
   if (Qt::EditRole != role && Qt::CheckStateRole != role)
      return result;

   const uint16_t row = index.row();

   QStandardItem* timeItem = item(row, index.column() + 1);
   const uint32_t position = value.toInt();
   const QString timeText = compileTime(bank, Tempo::Bar, position);
   timeItem->setText(timeText);

   stageList[row].position = position;

   return result;
}
