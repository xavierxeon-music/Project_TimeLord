#ifndef MainH
#define MainH

// base
#include "TimeLord.h"
#include <Abstract/AbstractPatch.h>
#include <Midi/MidiHandlerInternal.h>
#include <Storage/FlashSettings.h>

#include <Midi/MidiHandlerUSB.h>
#include <Midi/MidiInterfaceFlameCC.h>
#include <Music/ClockedTempo.h>

class Main : public Abstract::Patch, public FlashSettings, public Midi::Handler::Internal, public TimeLord
{
public:
   Main();

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

   void receiveControllerChangeMidi(const Midi::ControllerMessage& message, const uint8_t& value);

   // gates
   void clockTick();
   void clockReset();
private:
   ClockedTempo tempo;

   Midi::Handler::USB usbMidiHandler;
   Midi::Interface::FlameCC expander;
};

#endif // MainH
