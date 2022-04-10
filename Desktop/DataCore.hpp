#ifndef DataCoreHPP
#define DataCoreHPP

#include "DataCore.h"

template <typename... Args>
void Data::Core::callOnAllInstances(void (Core::*function)(Args...), Args... args)
{
   for (Core* instance : instanceList)
   {
      if (this == instance)
         continue;
      std::invoke(function, instance, args...);
   }
}

#endif // NOT DataCoreHPP
