#include "GraphDevice.h"

GraphDevice::GraphDevice(QObject* parent)
   : QObject(parent)
   , audioDriver("ES-8")
{
}

