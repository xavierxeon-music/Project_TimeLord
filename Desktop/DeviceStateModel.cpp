#include "DeviceStateModel.h"

#include "Target.h"

DeviceState::Model::Model(QObject* parent, Target* target)
   : QStandardItemModel(parent)
   , Core::Interface()
   , currentStage()
   , stageList()

{
   connect(target, &Target::signalNewState, this, &Model::slotNewState);
}

void DeviceState::Model::applyToBanks()
{
}

void DeviceState::Model::slotNewState(const QJsonObject& stateObject)
{
   qDebug() << stateObject << getBankCount();
}
