#include "RampDeviceVCV.h"

RampDevice::VCV::VCV(QObject* parent)
   : QObject(parent)
   , polyRamps{}
   , output(this, "TimeLordServer")
{
   connectToServer();
}

void RampDevice::VCV::pushToServer()
{
}

void RampDevice::VCV::connectToServer()
{
   output.open();
}
