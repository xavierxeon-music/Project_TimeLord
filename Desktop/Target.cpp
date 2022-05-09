#include "Target.h"

#include <QJsonArray>
#include <QJsonDocument>

#include <Midi/MidiCommon.h>
#include <Tools/SevenBit.h>

#include "DataCore.h"

static constexpr uint8_t remoteChannel = Midi::Device::VCVRack;

Target::Target(QObject* parent)
   : QObject(parent)
   , banks{}
   , output(this, "TimeLord")
   , actions{nullptr, nullptr, nullptr, nullptr}
{

   actions.connectToServer = new QAction(QIcon(":/Port.svg"), "Connect To Server", this);
   connect(actions.connectToServer, &QAction::triggered, this, &Target::slotConnectToServer);
   actions.connectToServer->setCheckable(true);

   actions.pushToServer = new QAction(QIcon(":/SaveToDaisy.svg"), "Push To Server", this);
   connect(actions.pushToServer, &QAction::triggered, this, &Target::slotPushToServer);

   slotConnectToServer(true);
}

const Target::ServerActions& Target::getServerActions() const
{
   return actions;
}

void Target::slotConnectToServer(bool connect)
{
   if (connect)
      output.open();
   else
      output.close();

   actions.connectToServer->setChecked(output.isOpen());
}

void Target::slotPushToServer()
{
   if (!output.isOpen())
      return;

   /*
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
   */
}

void Target::slotAddBank()
{
}

void Target::slotRemoveBank()
{
}
