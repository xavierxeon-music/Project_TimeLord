#ifndef MainH
#define MainH

// base
#include <Abstract/AbstractPatch.h>
#include <Midi/MidiHandlerInternal.h>
#include <Storage/FlashSettings.h>

#include <Blocks/PolyRamp.h>
#include <Midi/MidiHandlerUSB.h>
#include <Midi/MidiDeviceFlameCC.h>
#include <Music/ClockedTempo.h>

class Main : public Abstract::Patch, public FlashSettings, public Midi::Handler::Internal
{
public:
   Main();

private: // things to remeber
   using PolyRampList_ = Remember::RefArray<PolyRamp, 16>;

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

   void receiveControllerChangeMidi(const Midi::Channel& channel, const Midi::ControllerMessage& message, const uint8_t& value);

   // gates
   void clockTick();
   void clockReset();

private:
   PolyRampList_ ramps;
   ClockedTempo tempo;

   Midi::Handler::USB usbMidiHandler;
   Midi::Device::FlameCC expander;
};

#endif // MainH
