#include "GraphAudioDevice.h"

#include <QTimer>

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

   QTimer* statusUpdateTimer = new QTimer(this);
   connect(statusUpdateTimer, &QTimer::timeout, this, &GraphAudioDevice::slotStatusUpdate);
   statusUpdateTimer->start(1000);
}

void GraphAudioDevice::audioLoop(const float& audioCallbackRate)
{
   tempo.advance(audioCallbackRate);
}

void GraphAudioDevice::slotStatusUpdate()
{
   emit signalStatusUpdate(tempo.getRunState(), tempo.getBeatsPerMinute());
}
