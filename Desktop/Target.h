#ifndef TargetH
#define TargetH

#include <QObject>

#include <QAction>

#include <Midi/MidiPhysicalOutput.h>

#include "BankData.h"

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
      QAction* addBank;
      QAction* removeBank;
   };

public:
   Target(QObject* parent);

public:
   const ServerActions& getServerActions() const;

private:
   friend class Data::Core;

private slots:
   void slotConnectToServer(bool connect);
   void slotPushToServer();
   void slotAddBank();
   void slotRemoveBank();

private:
   std::vector<Bank::Data> banks;
   Midi::Physical::Output output;

   ServerActions actions;
};

#endif // NOT TargetH
