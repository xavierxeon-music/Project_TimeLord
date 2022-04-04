#ifndef RaspiDeviceGraphH
#define RaspiDeviceGraphH

#include <Remember.h>
#include <QObject>

#include <Blocks/PolyRamp.h>
#include <Midi/MidiToolBridge.h>
#include <Midi/MidiToolTempo.h>
#include <Midi/MidiTunnelClient.h>
#include <Midi/MidiVirtualOutput.h>

#include "DataCore.h"

namespace RampDevice
{
   class Raspi : public QObject, public Remember::Root
   {
      Q_OBJECT
   public:
      Raspi(QObject* parent);

   public:
      void pushToServer();
      void enableMidiPort(bool enabled);

   private:
      friend class Data::Core;
      using PolyRampList_ = Remember::RefArray<PolyRamp, 16>;

   private:
      PolyRampList_ polyRamps;
      Midi::Tunnel::Client client;
      Midi::Tool::Bridge bridge;
      Midi::Virtual::Output passThrough;
      Midi::Tool::Tempo tempo;
   };
} // namespace RampDevice

#endif // NOT RaspiDeviceGraphH
