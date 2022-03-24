#ifndef MainH
#define MainH

// base
#include <Abstract/AbstractPatch.h>
#include <Midi/MidiHandlerInternal.h>
#include <Storage/FlashSettings.h>

#include <Blocks/Graph.h>
#include <Midi/MidiHandlerUSB.h>
#include <Midi/MidiInterfaceFlameCC.h>
#include <Music/ClockedTempo.h>

class Main : public Abstract::Patch, public FlashSettings, public Midi::Handler::Internal
{
public:
   Main();

private: // things to remeber
   using GraphList_ = Remember::RefArray<Graph, 16>;

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

   void receiveControllerChangeMidi(const Midi::ControllerMessage& message, const uint8_t& value);

   // gates
   void clockTick();
   void clockReset();

private:
   GraphList_ graphs;
   ClockedTempo tempo;

   Midi::Handler::USB usbMidiHandler;
   Midi::Interface::FlameCC expander;
};

#endif // MainH
