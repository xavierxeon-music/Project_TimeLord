#include "BankData.h"

#include <QJsonArray>

#include "DataCore.h"

Bank::Data::Data()
   : polyRamps{}
{
}

QJsonObject Bank::Data::write() const
{
   QJsonObject ramps;
   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
   {
      const PolyRamp* polyRamp = &polyRamps[rampIndex];

      QJsonArray stageArray;
      for (uint8_t stageIndex = 0; stageIndex < polyRamp->getStageCount(); stageIndex++)
      {
         QJsonObject stageObject;
         stageObject["length"] = polyRamp->getStageLength(stageIndex);
         stageObject["startHeight"] = polyRamp->getStageStartHeight(stageIndex);
         stageObject["endHeight"] = polyRamp->getStageEndHeight(stageIndex);
         stageArray.append(stageObject);
      }

      QJsonObject rampObject;
      rampObject["stages"] = stageArray;
      rampObject["stepSize"] = polyRamp->getStepSize();
      rampObject["length"] = static_cast<qint64>(polyRamp->getLength());
      rampObject["loop"] = polyRamp->isLooping();

      const QString key = ::Data::Core::keys.at(rampIndex);
      ramps[key] = rampObject;
   }
   return ramps;
}

void Bank::Data::read(const QJsonObject& data) const
{
}
