#include "RampDeviceVCV.h"

#include <QJsonArray>
#include <QJsonDocument>

#include <Midi/MidiCommon.h>
#include <Tools/SevenBit.h>

#include "DataCore.h"

static constexpr uint8_t remoteChannel = Midi::Device::VCVRack;

RampDevice::VCV::VCV(QObject* parent)
   : QObject(parent)
   , Remember::Root()
   , polyRamps(this)
   , output(this, "TimeLord")
   , bankIndex(this, 0)
   , actions{nullptr, nullptr, nullptr}
   , iconBuffer()
{
   for (uint8_t index = 0; index < 10; index++)
      iconBuffer[index] = QIcon(":/Bank" + QString::number(index) + ".svg");

   actions.connectToServer = new QAction(QIcon(":/Port.svg"), "Connect To Server", this);
   actions.connectToServer->setCheckable(true);

   actions.bankUp = new QAction(iconBuffer[0], "Bank Up", this);
   connect(actions.bankUp, &QAction::triggered, this, &RampDevice::VCV::slotBankUp);

   actions.pushToServer = new QAction(QIcon(":/SaveToDaisy.svg"), "Push To Server", this);
   connect(actions.connectToServer, &QAction::triggered, this, &RampDevice::VCV::slotPushToServer);

   slotConnectToServer(true);
}

const RampDevice::VCV::ServerActions& RampDevice::VCV::getServerActions() const
{
   return actions;
}

void RampDevice::VCV::updateBankIcon()
{
   actions.bankUp->setIcon(iconBuffer[bankIndex]);
}

void RampDevice::VCV::slotConnectToServer(bool connect)
{
   if (connect)
      output.open();
   else
      output.close();

   actions.connectToServer->setChecked(output.isOpen());
}

void RampDevice::VCV::slotBankUp()
{
   bankIndex = bankIndex + 1;
   if (10 <= bankIndex)
      bankIndex = 0;

   setUnsynced();
   updateBankIcon();
}

void RampDevice::VCV::slotPushToServer()
{
   if (!output.isOpen())
      return;

   const QJsonObject ramps = compileRamps();
   qDebug() << ramps;

   const QJsonDocument document(ramps);
   const QByteArray content = document.toJson(QJsonDocument::Compact);

   Bytes data(content.size());
   std::memcpy(&data[0], content.constData(), content.size());

   Bytes dataBase64 = SevenBit::encode(data);

   qDebug() << content.size() << data.size() << dataBase64.size();

   for (const uint8_t byte : dataBase64)
      output.sendControllerChange(remoteChannel, Midi::ControllerMessage::RememberBlock, byte);

   output.sendControllerChange(remoteChannel, Midi::ControllerMessage::RememberApply, bankIndex);
}

QJsonObject RampDevice::VCV::compileRamps() const
{
   QJsonObject ramps;
   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
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
