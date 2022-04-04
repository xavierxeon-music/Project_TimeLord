#include "RampDeviceRaspi.h"

RampDevice::Raspi::Raspi(QObject* parent)
   : QObject(parent)
   , Remember::Root()
   , polyRamps(this)
   , client(parent, "192.168.1.180")
   , bridge(this, &client, &client, 11, 10)
   , output("TimeLordMirror")
   , tempo(&client)
{
   client.addPassThroughInterface(&output);
}

void RampDevice::Raspi::enableMidiPort(bool enabled)
{
   if (enabled)
      output.open();
   else
      output.close();
}

void RampDevice::Raspi::pushToServer()
{
   bridge.pushToRemote();
}

