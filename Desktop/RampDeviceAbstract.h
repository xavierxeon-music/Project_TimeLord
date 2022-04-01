#ifndef RampDeviceAbstractH
#define RampDeviceAbstractH

#include <Remember.h>

#include <Blocks/PolyRamp.h>

#include "DataCore.h"

namespace RampDevice
{
   class Abstract : public Remember::Root
   {
   public:
      Abstract();

   protected: // things to remeber
      using PolyRampList_ = Remember::RefArray<PolyRamp, 16>;

   protected:
      PolyRampList_ polyRamps;
   };
} // namespace RampDevice

#endif // NOT RampDeviceAbstractH
