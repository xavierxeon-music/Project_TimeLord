#include "DataCore.h"

#include "AudioDeviceGraph.h"
#include "MainWidget.h"

const DataCore::PoviderNameMap DataCore::providerNameMap = {{Model::Provider::DaisyPatch, "Daisy"}, {Model::Provider::AudioDeviceGraph, "ES-8"}};
bool DataCore::lockGraphSize = true;

DataCore::DataCore(MainWidget* mainWidget)
   : mainWidget(mainWidget)
{
}

DataCore::PoviderNameMap DataCore::getProviderNames() const
{
   return providerNameMap;
}

PolyRamp* DataCore::getPolyRamp(const Model::Provider& provider, const uint8_t& index)
{
   if (Model::Provider::DaisyPatch == provider)
      return &(mainWidget->polyRamps[index]);
   else if (Model::Provider::AudioDeviceGraph == provider)
      return &(mainWidget->audioDevice->polyRamps[index]);
   else
      return nullptr;
}
