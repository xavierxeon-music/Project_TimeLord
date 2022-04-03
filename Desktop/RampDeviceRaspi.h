#ifndef RaspiDeviceGraphH
#define RaspiDeviceGraphH

#include "RampDeviceAbstract.h"
#include <Midi/MidiTunnelClient.h>

#include <Midi/MidiDeviceDoepferQuad.h>

#include "DataCore.h"

#include <Sound/TableOscilator.h>

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

      TableOscilator one;
      TableOscilator two;
   };
} // namespace RampDevice

#endif // NOT RaspiDeviceGraphH
