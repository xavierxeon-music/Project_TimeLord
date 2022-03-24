#include "DataCore.h"

#include "MainWidget.h"

DataCore::DataCore(MainWidget* mainWidget)
   : mainWidget(mainWidget)
{
}

Graph& DataCore::graph(const Provider& provider, const uint8_t& index)
{
   if (Provider::DaisyPatch == provider)
      return mainWidget->graphs[index];
   else if (Provider::GraphDevice == provider)
      return
}
