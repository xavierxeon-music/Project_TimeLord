#include "DataCore.h"

#include "RampDeviceAudio.h"
#include "MainWidget.h"

Data::Identifier::Identifier(const Data::RampDevice& device, const uint8_t& rampIndex, const uint8_t& stageIndex)
   : device(device)
   , rampIndex(rampIndex)
   , stageIndex(stageIndex)
{
}

const Data::Core::DeviceNameMap Data::Core::deviceNameMap = {{RampDevice::Audio, "ES-8"}, {RampDevice::Raspi, "Raspi"}};
bool Data::Core::lockGraphSize = true;

Data::Core::Core(MainWidget* mainWidget)
   : mainWidget(mainWidget)
{
}

Data::Core::DeviceNameMap Data::Core::getDeviceNameMap() const
{
   return deviceNameMap;
}

PolyRamp* Data::Core::getPolyRamp(const Identifier& identifier)
{
   if (RampDevice::Raspi == identifier.device)
      return &(mainWidget->raspiDevice.polyRamps[identifier.rampIndex]);
   else if (RampDevice::Audio == identifier.device)
      return &(mainWidget->audioDevice.polyRamps[identifier.rampIndex]);
   else
      return nullptr;
}
