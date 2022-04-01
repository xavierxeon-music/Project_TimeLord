#ifndef MainObjectH
#define MainObjectH

#include <QObject>

#include <MidiTunnelServer.h>

class MainObject : public QObject
{
   Q_OBJECT
public:
   MainObject();
   ~MainObject();

private:
   Midi::Tunnel::Server server;
};

#endif // NOT MainObjectH
