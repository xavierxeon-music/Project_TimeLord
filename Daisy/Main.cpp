#include "Main.h"

#include <Abstract/AbstractPage.h>

#include "PageOverview.h"

Main::Main()
   : Abstract::Patch("TimeLord")
   , FlashSettings(this, 33)
   , Midi::Handler::Internal(&daisy, 1, this)
   , graphs(this)
   , tempo()
   , usbMidiHandler(this)
   , expander(&usbMidiHandler)
{
   usbMidiHandler.initMidi();
   usbMidiHandler.onReceiveControllChange(this, &Main::receiveControllerChangeMidi);

   load();

   for (uint8_t index = 0; index < graphs.getSize(); index++)
      graphs[index].clockReset();

   pageManager.addPage(new PageOverview(this, tempo));

   clockReset();
}

void Main::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   (void)in;
   (void)out;
   (void)size;

   usbMidiHandler.pollMidiReceive();

   // input
   const Input input = compileInput();
   const ClockedTempo::Event event = tempo.advance(input, daisy.AudioCallbackRate());
   if (ClockedTempo::Event::Reset == event)
      clockReset();
   else if (ClockedTempo::Event::Tick == event)
      clockTick();

   // process output
   Output output;
   for (uint8_t index = 0; index < 2; index++)
   {
      // TODO: graphs
   }

   const bool runningPastFirstTick = (Tempo::RunState::Running == tempo.getRunState());
   output.led = runningPastFirstTick;
   output.gate = runningPastFirstTick;
   applyOutput(output);

   const Encoder encoder = compileEncoder();
   updatePageManager(encoder);
}

void Main::nonAudioLoop()
{
   ensureSettignsValid();
}

void Main::receiveControllerChangeMidi(const Midi::ControllerMessage& message, const uint8_t& value)
{
   usbMidiHandler.settingsUpdate(this, message, value);
}

void Main::clockTick()
{
   for (uint8_t index = 0; index < graphs.getSize(); index++)
      graphs[index].clockTick();
}

void Main::clockReset()
{
   for (uint8_t index = 0; index < graphs.getSize(); index++)
      graphs[index].clockReset();
}

// main function

int main(void)
{
   Main app;
   app.run();
}
