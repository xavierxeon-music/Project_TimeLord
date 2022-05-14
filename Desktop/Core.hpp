#ifndef DataCoreHPP
#define DataCoreHPP

#include "Core.h"

template <typename... Args>
void Core::Interface::callOnAllInstances(void (Interface::*function)(Args...), Args... args)
{
   for (Interface* instance : instanceList)
   {
      if (this == instance)
         continue;
      std::invoke(function, instance, args...);
   }
}

#endif // NOT DataCoreHPP
