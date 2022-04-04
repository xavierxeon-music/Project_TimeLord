#include "DataCore.h"

#include "MainWidget.h"

Data::Identifier::Identifier(const uint8_t& rampIndex, const uint8_t& stageIndex)
   : rampIndex(rampIndex)
   , stageIndex(stageIndex)
{
}

bool Data::Core::lockGraphSize = true;

Data::Core::Core(MainWidget* mainWidget)
   : mainWidget(mainWidget)
{
}


PolyRamp* Data::Core::getPolyRamp(const Identifier& identifier)
{
   return &(mainWidget->raspiDevice.polyRamps[identifier.rampIndex]);
}
