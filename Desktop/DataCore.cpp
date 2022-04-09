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
RampDevice::Raspi* Data::Core::raspiDevice = nullptr;
QList<Data::Core*> Data::Core::instanceList = QList<Data::Core*>();

Data::Core::Core()
{
   instanceList.append(this);
}

void Data::Core::modelHasChanged(const Identifier& identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}

void Data::Core::polyRampSelected(const Identifier& identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}

void Data::Core::rebuildModel(const Identifier& identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}

PolyRamp* Data::Core::getPolyRamp(const Identifier& identifier)
{
   if (!raspiDevice)
      return nullptr;

   return &(raspiDevice->polyRamps[identifier.rampIndex]);
}

void Data::Core::callOnAllInstances(const Identifier& identifier, InstanceFunctionPointer instanceFunctionPointer)
{
   for (Core* instance : instanceList)
   {
      if (this == instance)
         continue;
      std::invoke(instanceFunctionPointer, instance, identifier);
   }
}

void Data::Core::setLockGraphSize(bool locked)
{
   lockGraphSize = locked;
}

void Data::Core::toggleLockGraphSize()
{
   lockGraphSize = !lockGraphSize;
}

bool Data::Core::getLockGraphSize() const
{
   return lockGraphSize;
}

void Data::Core::init(RampDevice::Raspi* raspiDevice)
{
   Core::raspiDevice = raspiDevice;
}
