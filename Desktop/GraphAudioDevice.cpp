#include "GraphAudioDevice.h"

#include <QTimer>

GraphAudioDevice::GraphAudioDevice(QObject* parent)
   : QObject(parent)
   , Remember::Root()
   , graphs(this)
   , audioDriver(AudioDevice::Common::Device::ES8, AudioDevice::Common::SampleRate::Normal)
   , tempo(&audioDriver, 0, 1)
   , outputs()
{
   audioDriver.registerAudioLoopFunction(this, &GraphAudioDevice::audioLoop);
   for (uint8_t index = 0; index < 16; index++)
      outputs[index].activate(&audioDriver, index);

   tempo.onClockTick(this, &GraphAudioDevice::clockTick);
   tempo.onClockReset(this, &GraphAudioDevice::clockReset);

   QTimer* statusUpdateTimer = new QTimer(this);
   connect(statusUpdateTimer, &QTimer::timeout, this, &GraphAudioDevice::slotStatusUpdate);
   statusUpdateTimer->start(1000);
}

void GraphAudioDevice::slotStatusUpdate()
{
   emit signalStatusUpdate(tempo.getRunState(), tempo.getBeatsPerMinute());
}

void GraphAudioDevice::audioLoop(const float& audioCallbackRate)
{
   tempo.advance(audioCallbackRate);

   for (uint8_t index = 0; index < 16; index++)
   {
      if (tempo.isRunningOrFirstTick())
      {
         const uint8_t value = graphs[index].getCurrentValue();
         const float voltage = value * 5.0 / 255.0;
         outputs[index].setVoltage(voltage);
      }
      else
      {
         outputs[index].setVoltage(0.0);
      }
   }
}

void GraphAudioDevice::clockTick()
{
   for (uint8_t index = 0; index < 16; index++)
      graphs[index].clockTick();
}

void GraphAudioDevice::clockReset()
{
   for (uint8_t index = 0; index < 16; index++)
      graphs[index].clockReset();
}
