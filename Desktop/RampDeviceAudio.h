#ifndef AudioDeviceGraphH
#define AudioDeviceGraphH

#include "RampDeviceAbstract.h"
#include <QObject>

#include <AudioDevice.h>

#include "DataCore.h"

namespace RampDevice
{
   class Audio : public QObject, public Abstract
   {
      Q_OBJECT
   public:
      Audio(QObject* parent);

   public:
      void clockReset();
      const Tempo* getTempo() const;

   private:
      friend class Data::Core;

   private:
      void audioLoop(const float& audioCallbackRate);
      void clockTick();

   private:
      AudioDevice::Driver audioDriver;
      AudioDevice::InputsTempo tempo;
      AudioDevice::OutputCV outputs[16];
   };
} // namespace RampDevice

#endif // NOT AudioDeviceGraphH
