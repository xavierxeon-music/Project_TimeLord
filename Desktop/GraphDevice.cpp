#include "GraphDevice.h"

GraphDevice::GraphDevice(QObject* parent)
   : QObject(parent)
   , Remember::Root()
   , graphs(this)
   , audioDriver("ES-8")
{
}

