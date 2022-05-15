#include "Core.h"

#include "MainWidget.h"
#include "Target.h"

// identifier

Core::Identifier::Identifier(const uint8_t& bankIndex, const uint8_t& rampIndex, const uint8_t& stageIndex)
   : bankIndex(bankIndex)
   , rampIndex(rampIndex)
   , stageIndex(stageIndex)
{
}

bool Core::Identifier::hasStage() const
{
   return (255 != stageIndex);
}

// type

QString Core::Type::getName(const Value& type)
{
   if (Core::Type::StartAnchor == type)
      return "Anchor";
   else if (Core::Type::EndAnchor == type)
      return "Anchor";
   else if (Core::Type::Rise == type)
      return "Rise";
   else if (Core::Type::Fall == type)
      return "Fall";
   else if (Core::Type::Stable == type)
      return "Stable";
   else if (Core::Type::Step == type)
      return "Step";
   else
      return "Inkown";
}

QIcon Core::Type::getIcon(const Value& type)
{
   if (Core::Type::StartAnchor == type)
      return QIcon(":/TrendAnchor.svg");
   else if (Core::Type::EndAnchor == type)
      return QIcon(":/TrendAnchor.svg");
   else if (Core::Type::Rise == type)
      return QIcon(":/TrendRise.svg");
   else if (Core::Type::Fall == type)
      return QIcon(":/TrendFall.svg");
   else if (Core::Type::Stable == type)
      return QIcon(":/TrendStable.svg");
   else if (Core::Type::Step == type)
      return QIcon(":/TrendStep.svg");
   else
      return QIcon();
}

// core

const QString Core::Interface::keys = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
bool Core::Interface::isModified = false;
bool Core::Interface::lockGraphSize = true;
Target* Core::Interface::target = nullptr;
QList<Core::Interface*> Core::Interface::instanceList = QList<Core::Interface*>();

Core::Interface::Interface()
{
   instanceList.append(this);
}

void Core::Interface::modelHasChanged(Identifier identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}

void Core::Interface::selectionChanged(Identifier identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}

void Core::Interface::rebuildModel(Identifier identifier)
{
   Q_UNUSED(identifier)
   // do nothing
}

uint8_t Core::Interface::getBankCount() const
{
   if (!target)
      return 0;

   return target->banks.size();
}

void Core::Interface::addBank()
{
   if (!target)
      return;

   if (127 == target->banks.size())
      return;

   target->banks.push_back(new Bank::Content());
}

void Core::Interface::removeBank()
{
   if (!target)
      return;

   if (target->banks.empty())
      return;

   target->banks.pop_back();
}

Bank::Content* Core::Interface::getBank(const Identifier& identifier)
{
   if (!target)
      return nullptr;

   return target->banks[identifier.bankIndex];
}

const Bank::Content* Core::Interface::getBank(const Identifier& identifier) const
{
   if (!target)
      return nullptr;

   return target->banks[identifier.bankIndex];
}

PolyRamp* Core::Interface::getPolyRamp(const Identifier& identifier)
{
   if (!target)
      return nullptr;

   if (identifier.bankIndex >= target->banks.size())
      return nullptr;

   Bank::Content* bankContent = target->banks[identifier.bankIndex];
   return &(bankContent->polyRamps[identifier.rampIndex]);
}

const PolyRamp* Core::Interface::getPolyRamp(const Identifier& identifier) const
{
   if (!target)
      return nullptr;

   if (identifier.bankIndex >= target->banks.size())
      return nullptr;

   const Bank::Content* bankContent = target->banks.at(identifier.bankIndex);
   return &(bankContent->polyRamps[identifier.rampIndex]);
}

void Core::Interface::setLockGraphSize(bool locked)
{
   lockGraphSize = locked;
}

void Core::Interface::toggleLockGraphSize()
{
   lockGraphSize = !lockGraphSize;
}

bool Core::Interface::getLockGraphSize() const
{
   return lockGraphSize;
}

void Core::Interface::setModified()
{
   isModified = true;
}

void Core::Interface::createRampDevice(QObject* parent)
{
   if (target)
      return;

   target = new ::Target(parent);
}

void Core::Interface::unsetModified()
{
   isModified = false;
}
