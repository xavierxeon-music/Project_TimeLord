#include "RampDeviceRaspi.h"

RampDevice::Raspi::Raspi(QObject* parent)
   : QObject(parent)
   , Remember::Root()
   , polyRamps(this)
   , client(parent, "192.168.1.180")
   , bridge(this, &client, &client, 11, 10)
   , passThrough(this, "TimeLordMirror")
   , tempo(&client)
{
   client.addPassThroughInterface(&passThrough);
}

void RampDevice::Raspi::enableMidiPort(bool enabled)
{
   if (enabled)
      passThrough.open();
   else
      passThrough.close();
}

void RampDevice::Raspi::pushToServer()
{
   bridge.pushToRemote();
}

