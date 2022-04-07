#include "DataCore.h"

#include "MainWidget.h"

// identifier

Data::Identifier::Identifier(const uint8_t& rampIndex, const uint8_t& stageIndex)
   : rampIndex(rampIndex)
   , stageIndex(stageIndex)
{
}

bool Data::Identifier::hasStage() const
{
   return (255 != stageIndex);
}

// core

bool Data::Core::lockGraphSize = true;
QList<Data::Core*> Data::Core::instanceList = QList<Data::Core*>();

Data::Core::Core(MainWidget* mainWidget)
   : mainWidget(mainWidget)
   , instanceList.append(this){}
{
}

PolyRamp* Data::Core::getPolyRamp(const Identifier& identifier)
{
   return &(mainWidget->raspiDevice.polyRamps[identifier.rampIndex]);
}

void Data::Core::sendModelChanged(const Identifier& identifier)
{
   for (Core* instance : instanceList)
   {
      if (this == instance)
         continue;
      instance->modelHasChanged(identifier);
   }
}

void Data::Core::modelHasChanged(const Identifier& identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}
