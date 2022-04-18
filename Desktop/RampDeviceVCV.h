#ifndef RampDeviceVCVH
#define RampDeviceVCVH

#include <Blocks/PolyRamp.h>

#include <Midi/MidiPhysicalOutput.h>

namespace RampDevice
{
   class VCV : public QObject
   {
      Q_OBJECT
   public:
      VCV(QObject* parent);

   public:
      void pushToServer();
      void connectToServer();

   private:
      PolyRamp polyRamps[16];
      Midi::Physical::Output output;
   };
} // namespace RampDevice

#endif // NOT RampDeviceVCVH
