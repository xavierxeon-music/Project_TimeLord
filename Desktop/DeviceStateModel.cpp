#include "DeviceStateModel.h"

#include "Target.h"

DeviceState::Model::Model(QObject* parent, Target* target)
   : QStandardItemModel(parent)
   , Core::Interface()
   , currentStage()
   , stageList()

{
   connect(target, &Target::signalNewState, this, &Model::slotNewState);

   setHorizontalHeaderLabels({"number", "start pos", "time"});
}

void DeviceState::Model::applyToBanks()
{
}

QString DeviceState::Model::compileInfo() const
{
   QString info;
   info += QString::number(getBankCount()) + " banks";

   Core::Identifier dummy;
   const Bank::Content* bank = getBank(dummy);
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

   QStandardItem* numberItem = new QStandardItem();
   uint16_t index = invisibleRootItem()->rowCount();
   numberItem->setText(QString::number(index));
   numberItem->setEditable(false);

   QStandardItem* posItem = new QStandardItem();
   posItem->setText("0");

   QStandardItem* timeItem = new QStandardItem();
   timeItem->setText("0");

   invisibleRootItem()->appendRow({numberItem, posItem, timeItem});
   currentStage.map.clear();
}
