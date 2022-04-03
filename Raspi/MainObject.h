#ifndef MainObjectH
#define MainObjectH

#include <QObject>

#include <Midi/MidiDeviceInput.h>
#include <Midi/MidiDeviceOutput.h>
#include <Midi/MidiTunnelServer.h>

class MainObject : public QObject
{
   Q_OBJECT
public:
   MainObject();
   ~MainObject();

private:
   Midi::Tunnel::Server server;
   Midi::Device::Input inputDevice;
   Midi::Device::Output outputDevice;
};

#endif // NOT MainObjectH
