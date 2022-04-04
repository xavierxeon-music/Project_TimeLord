#ifndef MainObjectH
#define MainObjectH

#include <Remember.h>
#include <QObject>

#define WAIT_FOR_FLAME_DEVICE

#ifdef WAIT_FOR_FLAME_DEVICE
#include <Midi/MidiTargetDoepferQuad.h> // temporary ?
#endif

#include <Blocks/PolyRamp.h>
#include <Midi/MidiDeviceInput.h>
#include <Midi/MidiDeviceOutput.h>
#include <Midi/MidiTargetFlameCC.h>
#include <Midi/MidiToolBridge.h>
#include <Midi/MidiToolGate.h>
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
   void reset();
   void receviedSettings();

private:
   PolyRampList_ polyRamps;
   static const QString storageFileName;

   Midi::Tunnel::Server server;

   Midi::Device::Input inputDevice;
   Midi::Device::Output outputDevice;

   Midi::Tool::Bridge bridge;

#ifdef WAIT_FOR_FLAME_DEVICE
   Midi::Target::DoepferQuad doepferQuad; // use for first 8 ramps
   Midi::Target::DoepferQuad::Strip quadStrips[4];
#endif
   Midi::Target::FlameCC flameCC;

   static const uint16_t callbackRate;
   Tempo tempo;
   Midi::Tool::Gate clockGate;
   Midi::Tool::Gate resetGate;
   bool tickActive;
};

#endif // NOT MainObjectH
