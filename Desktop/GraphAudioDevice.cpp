#include "GraphAudioDevice.h"

GraphAudioDevice::GraphAudioDevice(QObject* parent)
   : QObject(parent)
   , Remember::Root()
   , graphs(this)
   , audioDriver("ES-8")
{
}

