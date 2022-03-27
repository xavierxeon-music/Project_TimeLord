#include "DataCore.h"

#include "GraphAudioDevice.h"
#include "MainWidget.h"

const DataCore::PoviderNameMap DataCore::providerNameMap = {{Model::Provider::DaisyPatch, "Daisy"}, {Model::Provider::GraphAudioDevice, "ES-8"}};

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
   else if (Model::Provider::GraphAudioDevice == provider)
      return &(mainWidget->graphAudioDevice->graphs[index]);
   else
      return nullptr;
}
