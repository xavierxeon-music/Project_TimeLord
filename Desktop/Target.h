#ifndef TargetH
#define TargetH

#include <QObject>

#include <QAction>

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

private:
   std::vector<Bank::Content*> banks;
   Midi::Physical::Output output;

   ServerActions serverActions;
};

#endif // NOT TargetH
