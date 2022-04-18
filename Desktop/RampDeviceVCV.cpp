#include "RampDeviceVCV.h"

#include <QJsonArray>
#include <QJsonDocument>

#include <Midi/MidiCommon.h>
#include <Tools/SevenBit.h>

#include "DataCore.h"

static constexpr uint8_t remoteChannel = 11;

RampDevice::VCV::VCV(QObject* parent)
   : QObject(parent)
   , Remember::Root()
   , polyRamps(this)
   , output(this, "TimeLordServer")
{
   connectToServer();
}

QJsonObject RampDevice::VCV::compileRamps() const
{
   QJsonObject ramps;
   for (uint8_t rampIndex = 0; rampIndex < 16; rampIndex++)
   {
      const PolyRamp* polyRamp = &polyRamps[rampIndex];

      QJsonArray stageArray;
      for (uint8_t stageIndex = 0; stageIndex < polyRamp->stageCount(); stageIndex++)
      {
         QJsonObject stageObject;
         stageObject["length"] = polyRamp->getStageLength(stageIndex);
         stageObject["startHeight"] = polyRamp->getStageStartHeight(stageIndex);
         stageArray.append(stageObject);
      }

      QJsonObject rampObject;
      rampObject["stages"] = stageArray;
      rampObject["stepSize"] = polyRamp->getStepSize();
      rampObject["length"] = static_cast<int64_t>(polyRamp->getLength());
      rampObject["lopp"] = polyRamp->isLooping();

      const QString key = Data::Core::keys.at(rampIndex);
      ramps[key] = rampObject;
   }
   return ramps;
}

void RampDevice::VCV::pushToServer()
{
   const QJsonObject ramps = compileRamps();

   const QJsonDocument document(ramps);
   const QByteArray content = document.toJson(QJsonDocument::Compact);

   Bytes data(content.size());
   std::memcpy(&data[0], content.constData(), content.size());

   Bytes dataBase64 = SevenBit::encode(data);

   for (const uint8_t byte : dataBase64)
      output.sendControllerChange(remoteChannel, Midi::ControllerMessage::RememberBlock, byte);

   output.sendControllerChange(remoteChannel, Midi::ControllerMessage::RememberApply, 0);
}

void RampDevice::VCV::connectToServer()
{
   output.open();
}
