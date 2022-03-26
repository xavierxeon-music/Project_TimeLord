#include "GraphAudioDevice.h"

GraphAudioDevice::GraphAudioDevice(QObject* parent)
   : QObject(parent)
   , Remember::Root()
   , graphs(this)
   , audioDriver(AudioDevice::Common::DeviceES8, AudioDevice::Common::SampleRateNormal)
   , tempo(&audioDriver, 0, 1)
   , outputs()
{
   audioDriver.registerAudioLoopFunction(this, &GraphAudioDevice::audioLoop);
   for (uint8_t index = 0; index < 16; index++)
      outputs[index].activate(&audioDriver, index);
}

void GraphAudioDevice::audioLoop(const float& audioCallbackRate)
{
   tempo.advance(audioCallbackRate);
}
