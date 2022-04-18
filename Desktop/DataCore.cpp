#include "DataCore.h"

#include "MainWidget.h"
#include "RampDeviceVCV.h"

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

// type

QString Data::Type::getName(const Value& type)
{
   if (Data::Type::StartAnchor == type)
      return "Anchor";
   else if (Data::Type::EndAnchor == type)
      return "Anchor";
   else if (Data::Type::Rise == type)
      return "Rise";
   else if (Data::Type::Fall == type)
      return "Fall";
   else if (Data::Type::Stable == type)
      return "Stable";
   else if (Data::Type::Step == type)
      return "Step";
   else
      return "Inkown";
}

QIcon Data::Type::getIcon(const Value& type)
{
   if (Data::Type::StartAnchor == type)
      return QIcon(":/TrendAnchor.svg");
   else if (Data::Type::EndAnchor == type)
      return QIcon(":/TrendAnchor.svg");
   else if (Data::Type::Rise == type)
      return QIcon(":/TrendRise.svg");
   else if (Data::Type::Fall == type)
      return QIcon(":/TrendFall.svg");
   else if (Data::Type::Stable == type)
      return QIcon(":/TrendStable.svg");
   else if (Data::Type::Step == type)
      return QIcon(":/TrendStep.svg");
   else
      return QIcon();
}

// core

const QString Data::Core::keys = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
bool Data::Core::isModified = false;
bool Data::Core::lockGraphSize = true;
RampDevice::VCV* Data::Core::device = nullptr;
QList<Data::Core*> Data::Core::instanceList = QList<Data::Core*>();

Data::Core::Core()
{
   instanceList.append(this);
}

void Data::Core::modelHasChanged(Identifier identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}

void Data::Core::polyRampSelected(Identifier identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}

void Data::Core::rebuildModel(Identifier identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}

void Data::Core::saveSettings()
{
   // do nothing
}

PolyRamp* Data::Core::getPolyRamp(const Identifier& identifier)
{
   if (!device)
      return nullptr;

   return &(device->polyRamps[identifier.rampIndex]);
}

const PolyRamp* Data::Core::getPolyRamp(const Identifier& identifier) const
{
   if (!device)
      return nullptr;

   return &(device->polyRamps[identifier.rampIndex]);
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

void Data::Core::setModified()
{
   isModified = true;
}

void Data::Core::createRampDevice(QObject* parent)
{
   if (device)
      return;

   device = new RampDevice::VCV(parent);
}

void Data::Core::unsetModified()
{
   isModified = false;
}
