#include "RampDeviceAudio.h"

RampDevice::Audio::Audio(QObject* parent)
   : QObject(parent)
   , Abstract()
   , audioDriver(AudioDevice::Common::Device::ES8, AudioDevice::Common::SampleRate::Normal)
   , tempo(&audioDriver, 0, 1)
   , outputs()
{
   audioDriver.registerAudioLoopFunction(this, &RampDevice::Audio::audioLoop);
   for (uint8_t index = 0; index < 16; index++)
      outputs[index].activate(&audioDriver, index);

   tempo.onClockTick(this, &RampDevice::Audio::clockTick);
   tempo.onClockReset(this, &RampDevice::Audio::clockReset);
}

void RampDevice::Audio::clockReset()
{
   for (uint8_t index = 0; index < 16; index++)
      polyRamps[index].clockReset();
}

const Tempo* RampDevice::Audio::getTempo() const
{
   return &tempo;
}

void RampDevice::Audio::audioLoop(const float& audioCallbackRate)
{
   tempo.advance(audioCallbackRate);

   for (uint8_t index = 0; index < 16; index++)
   {
      if (tempo.isRunningOrFirstTick())
      {
         const Tempo::Division division = polyRamps[index].getStepSize();
         const uint8_t value = polyRamps[index].getCurrentValue(tempo.getPercentage(division));
         const float voltage = value * 5.0 / 255.0;
         outputs[index].setVoltage(voltage);
      }
      else
      {
         outputs[index].setVoltage(0.0);
      }
   }
}

void RampDevice::Audio::clockTick()
{
   for (uint8_t index = 0; index < 16; index++)
      polyRamps[index].clockTick();
}
