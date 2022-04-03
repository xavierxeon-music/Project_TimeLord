#ifndef RaspiDeviceGraphH
#define RaspiDeviceGraphH

#include "RampDeviceAbstract.h"
#include <QObject>

#include <Midi/MidiToolBridge.h>
#include <Midi/MidiTunnelClient.h>

#include "DataCore.h"

namespace RampDevice
{
   class Raspi : public QObject, public Abstract
   {
      Q_OBJECT
   public:
      Raspi(QObject* parent);

   public:
      void pushToServer();

   private:
      friend class Data::Core;

   private:
      Midi::Tunnel::Client client;
      Midi::Tool::Bridge bridge;
   };
} // namespace RampDevice

#endif // NOT RaspiDeviceGraphH
