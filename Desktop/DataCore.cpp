#include "DataCore.h"

#include "AudioDeviceGraph.h"
#include "MainWidget.h"

Data::Identifier::Identifier(const Data::Provider& provider, const uint8_t& rampIndex, const uint8_t& stageIndex)
   : provider(provider)
   , rampIndex(rampIndex)
   , stageIndex(stageIndex)
{
}

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

PolyRamp* Data::Core::getPolyRamp(const Identifier& identifier)
{
   if (Provider::DaisyPatch == identifier.provider)
      return &(mainWidget->polyRamps[identifier.rampIndex]);
   else if (Provider::AudioDeviceGraph == identifier.provider)
      return &(mainWidget->audioDevice->polyRamps[identifier.rampIndex]);
   else
      return nullptr;
}
