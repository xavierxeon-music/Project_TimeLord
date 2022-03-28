#include "DataCore.h"

#include "AudioDeviceGraph.h"
#include "MainWidget.h"

const DataCore::PoviderNameMap DataCore::providerNameMap = {{Model::Provider::DaisyPatch, "Daisy"}, {Model::Provider::AudioDeviceGraph, "ES-8"}};

DataCore::DataCore(MainWidget* mainWidget)
   : mainWidget(mainWidget)
{
}

DataCore::PoviderNameMap DataCore::getProviderNames() const
{
   return providerNameMap;
}

Graph* DataCore::getGraph(const Model::Provider& provider, const uint8_t& index)
{
   if (Model::Provider::DaisyPatch == provider)
      return &(mainWidget->graphs[index]);
   else if (Model::Provider::AudioDeviceGraph == provider)
      return &(mainWidget->audioDevice->graphs[index]);
   else
      return nullptr;
}
