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
   , output(this, "TimeLord")
   , serverActions{nullptr, nullptr}
{
   serverActions.connectToServer = new QAction(QIcon(":/Port.svg"), "Connect To Server", this);
   connect(serverActions.connectToServer, &QAction::triggered, this, &Target::slotConnectToServer);
   serverActions.connectToServer->setCheckable(true);

   serverActions.pushToServer = new QAction(QIcon(":/SaveToDaisy.svg"), "Push To Server", this);
   connect(serverActions.pushToServer, &QAction::triggered, this, &Target::slotPushToServer);

   slotConnectToServer(true);

}

const Target::ServerActions& Target::getServerActions() const
{
   return serverActions;
}

void Target::slotConnectToServer(bool connect)
{
   if (connect)
      output.open();
   else
      output.close();

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
