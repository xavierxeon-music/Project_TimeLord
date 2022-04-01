#ifndef MainObjectH
#define MainObjectH

#include <QObject>

#include <MidiDevice.h>
#include <MidiTunnelServer.h>

class MainObject : public QObject
{
   Q_OBJECT
public:
   MainObject();
   ~MainObject();

private:
   Midi::Tunnel::Server server;
   Midi::Device device;
};

#endif // NOT MainObjectH
