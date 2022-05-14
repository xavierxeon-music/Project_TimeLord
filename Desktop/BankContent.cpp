#include "BankContent.h"

#include <QJsonArray>

#include "Core.h"

Bank::Content::Content()
   : beatsPerMinute(120)
   , polyRamps{}
{
}

const uint8_t& Bank::Content::getBeatsPerMinute() const
{
   return beatsPerMinute;
}

QJsonObject Bank::Content::write() const
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

      const QString key = Core::Interface::keys.at(rampIndex);
      ramps[key] = rampObject;
   }
   return ramps;
}

void Bank::Content::read(const QJsonObject& data) const
{
   Q_UNUSED(data)
}
