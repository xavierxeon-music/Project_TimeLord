#ifndef TargetH
#define TargetH

#include <QObject>

#include <QAction>

#include <Midi/MidiPhysicalInput.h>
#include <Midi/MidiPhysicalOutput.h>

#include "BankContent.h"

namespace Data
{
   class Core;
}

class Target : public QObject
{
   Q_OBJECT
public:
   struct ServerActions
   {
      QAction* connectToServer;
      QAction* pushToServer;
      QAction* stateFromServer;
   };

public:
   Target(QObject* parent);

public:
   const ServerActions& getServerActions() const;

private:
   friend class Core::Interface;

private slots:
   void slotConnectToServer(bool connect);
   void slotPushToServer();
   void slotRequestStateFromServer();

private:
   void controllerChange(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value);

private:
   std::vector<Bank::Content*> banks;
   Midi::Physical::Output output;
   Midi::Physical::Input input;

   ServerActions serverActions;
   QByteArray midiBuffer;
};

#endif // NOT TargetH
