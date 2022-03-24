#include "DataCore.h"

#include "GraphDevice.h"
#include "MainWidget.h"

const DataCore::PoviderNameMap DataCore::providerNameMap = {{DataCore::Provider::DaisyPatch, "Daisy"}, {DataCore::Provider::GraphDevice, "ES8"}};

DataCore::DataCore(MainWidget* mainWidget)
   : mainWidget(mainWidget)
{
}

DataCore::PoviderNameMap DataCore::getProviderNames() const
{
   return providerNameMap;
}

Graph& DataCore::graph(const Provider& provider, const uint8_t& index)
{
   if (Provider::DaisyPatch == provider)
      return mainWidget->graphs[index];

   return mainWidget->graphDevice->graphs[index];
}
