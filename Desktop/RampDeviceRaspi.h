#ifndef RaspiDeviceGraphH
#define RaspiDeviceGraphH

#include "RampDeviceAbstract.h"
#include <MidiTunnelClient.h>

#include <Midi/MidiDeviceDoepferQuad.h>

#include "DataCore.h"

namespace RampDevice
{
   class Raspi : public Midi::Tunnel::Client, public Abstract
   {
      Q_OBJECT
   public:
      Raspi(QObject* parent);

   public:
      void pushToServer();

   private slots:
      void slotAdvanceStep();

   private:
      friend class Data::Core;

   private:
      Midi::Device::DoepferQuad midiDevice;
      Midi::Device::DoepferQuad::Strip stripA;
   };
} // namespace RampDevice

#endif // NOT RaspiDeviceGraphH
