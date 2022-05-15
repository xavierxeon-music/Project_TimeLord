#include "BankContent.h"

#include <QJsonArray>

#include "Core.h"

Bank::Content::Content()
   : beatsPerMinute(120)
   , polyRamps{}
   , rampNames{"1", "2", "3", "4", "5", "6", "7", "8"}
{
}

const uint8_t& Bank::Content::getBeatsPerMinute() const
{
   return beatsPerMinute;
}

void Bank::Content::setBeatsPerMinute(const uint8_t bpm)
{
   beatsPerMinute = bpm;
}

const QString& Bank::Content::getName(const uint8_t index) const
{
   return rampNames[index];
}

void Bank::Content::setName(const uint8_t index, const QString& name)
{
   rampNames[index] = name;
}

QJsonObject Bank::Content::writeNames() const
{
   QJsonObject names;
   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
   {
      const QString key = Core::Interface::keys.at(rampIndex);
      names[key] = rampNames[rampIndex];
   }

   return names;
}

void Bank::Content::readNames(const QJsonObject& data)
{
   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
   {
      const QString key = Core::Interface::keys.at(rampIndex);
      rampNames[rampIndex] = data[key].toString();
   }
}

QJsonObject Bank::Content::writeRamps() const
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
      rampObject["stepSize"] = static_cast<int>(polyRamp->getStepSize());
      rampObject["length"] = static_cast<qint64>(polyRamp->getLength());
      rampObject["loop"] = polyRamp->isLooping();

      const QString key = Core::Interface::keys.at(rampIndex);
      ramps[key] = rampObject;
   }
   return ramps;
}

void Bank::Content::readRamps(const QJsonObject& data)
{
   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
   {
      const QString key = Core::Interface::keys.at(rampIndex);
      const QJsonObject rampObject = data[key].toObject();

      PolyRamp* polyRamp = &polyRamps[rampIndex];
      polyRamp->clear();

      Tempo::Division stepSize = static_cast<Tempo::Division>(rampObject["stepSize"].toInt());
      polyRamp->setStepSize(stepSize);
      polyRamp->setLength(rampObject["length"].toInt());
      polyRamp->setLooping(rampObject["loop"].toBool());

      const QJsonArray stageArray = rampObject["stages"].toArray();
      polyRamp->addStage(0, stageArray.count());
      for (uint8_t stageIndex = 0; stageIndex < stageArray.count(); stageIndex++)
      {
         QJsonObject stageObject = stageArray.at(stageIndex).toObject();

         polyRamp->setStageLength(stageIndex, stageObject["length"].toInt());
         polyRamp->setStageStartHeight(stageIndex, stageObject["startHeight"].toInt());
         polyRamp->setStageEndHeight(stageIndex, stageObject["endHeight"].toInt());
      }
   }
}
