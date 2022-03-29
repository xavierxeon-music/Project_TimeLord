#include "DataCore.h"

#include "AudioDeviceGraph.h"
#include "MainWidget.h"

const Data::Core::PoviderNameMap Data::Core::providerNameMap = {{Provider::DaisyPatch, "Daisy"}, {Provider::AudioDeviceGraph, "ES-8"}};
bool Data::Core::lockGraphSize = true;

Data::Core::Core(MainWidget* mainWidget)
   : mainWidget(mainWidget)
{
}

Data::Core::PoviderNameMap Data::Core::getProviderNames() const
{
   return providerNameMap;
}

PolyRamp* Data::Core::getPolyRamp(const Provider& provider, const uint8_t& index)
{
   if (Provider::DaisyPatch == provider)
      return &(mainWidget->polyRamps[index]);
   else if (Provider::AudioDeviceGraph == provider)
      return &(mainWidget->audioDevice->polyRamps[index]);
   else
      return nullptr;
}
