#ifndef MainObjectH
#define MainObjectH

#include <Remember.h>
#include <QObject>

#include <Blocks/PolyRamp.h>
#include <Midi/MidiDeviceInput.h>
#include <Midi/MidiDeviceOutput.h>
#include <Midi/MidiTargetDoepferQuad.h> // temporary ?
#include <Midi/MidiTargetFlameCC.h>
#include <Midi/MidiTunnelServer.h>
#include <Music/Tempo.h>

class MainObject : public QObject, public Remember::Root
{
   Q_OBJECT
public:
   MainObject();
   ~MainObject();

private: // things to remeber
   using PolyRampList_ = Remember::RefArray<PolyRamp, 16>;

private:
   void loop();
   void clockTick();
   void clockReset();

private:
   PolyRampList_ polyRamps;

   Midi::Tunnel::Server server;
   Midi::Device::Input inputDevice;
   Midi::Device::Output outputDevice;

   Midi::Target::DoepferQuad doepferQuad; // use first 8 ramps
   Midi::Target::FlameCC flameCC;         // last 8 ramps go here

   static const float callbackRate;
   Tempo tempo;
};

#endif // NOT MainObjectH
