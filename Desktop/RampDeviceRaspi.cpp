#include "RampDeviceRaspi.h"

RampDevice::Raspi::Raspi(QObject* parent)
   : QObject(parent)
   , Abstract()
   , client(parent, "192.168.1.180")
   , bridge(this, &client, &client, 11, 10)
{
}

void RampDevice::Raspi::pushToServer()
{
   bridge.pushToRemote();
}

