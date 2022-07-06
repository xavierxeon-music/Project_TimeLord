#include "Target.h"

#include <QJsonArray>
#include <QJsonDocument>

#include <Midi/MidiCommon.h>
#include <Tools/SevenBit.h>

#include "Core.h"

static constexpr uint8_t remoteChannel = Midi::Device::VCVRack;

Target::Target(QObject* parent)
   : QObject(parent)
   , banks{}
   , output(this, "Majordomo")
   , input(this, "Majordomo")
   , serverActions{nullptr, nullptr, nullptr}
   , midiBuffer()
{
   serverActions.connectToServer = new QAction(QIcon(":/Port.svg"), "Connect To Server", this);
   connect(serverActions.connectToServer, &QAction::triggered, this, &Target::slotConnectToServer);
   serverActions.connectToServer->setCheckable(true);

   serverActions.pushToServer = new QAction(QIcon(":/SaveToMajordomo.svg"), "Push To Rack", this);
   connect(serverActions.pushToServer, &QAction::triggered, this, &Target::slotPushToServer);

   serverActions.stateFromServer = new QAction(QIcon(":/StateFromMajordomo.svg"), "State From Rack", this);
   connect(serverActions.stateFromServer, &QAction::triggered, this, &Target::slotRequestStateFromServer);

   input.onControllChange(this, &Target::controllerChange);
   slotConnectToServer(true);
}

const Target::ServerActions& Target::getServerActions() const
{
   return serverActions;
}

void Target::slotConnectToServer(bool connect)
{
   if (connect)
   {
      output.open();
      input.open();
   }
   else
   {
      output.close();
      input.close();
   }

   serverActions.connectToServer->setChecked(output.isOpen());
}

void Target::slotPushToServer()
{
   if (!output.isOpen())
      return;

   for (uint8_t bankIndex = 0; bankIndex < banks.size(); bankIndex++)
   {
      const QJsonObject ramps = banks[bankIndex]->writeRamps();
      //qDebug() << ramps;

      const QJsonDocument document(ramps);
      const QByteArray content = document.toJson(QJsonDocument::Compact);

      Bytes data(content.size());
      std::memcpy(&data[0], content.constData(), content.size());

      Bytes dataBase64 = SevenBit::encode(data);

      //qDebug() << content.size() << data.size() << dataBase64.size();

      for (const uint8_t byte : dataBase64)
         output.sendControllerChange(remoteChannel, Midi::ControllerMessage::RememberBlock, byte);

      output.sendControllerChange(remoteChannel, Midi::ControllerMessage::RememberApply, bankIndex);
   }
}

void Target::slotRequestStateFromServer()
{
   output.sendControllerChange(remoteChannel, Midi::ControllerMessage::RememberRequest, 0);
}

void Target::controllerChange(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value)
{
   if (Midi::Device::VCVRack != channel)
      return;

   if (Midi::ControllerMessage::RememberBlock == controllerMessage)
   {
      midiBuffer.append(value);
   }
   else if (Midi::ControllerMessage::RememberApply == controllerMessage)
   {
      const uint8_t bankIndex = value;
      const QJsonDocument document = QJsonDocument::fromJson(midiBuffer);

      midiBuffer.clear();

      qDebug() << bankIndex << document.object();
   }
}
