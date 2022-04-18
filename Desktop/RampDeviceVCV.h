#ifndef RampDeviceVCVH
#define RampDeviceVCVH

#include <Remember.h>
#include <QObject>

#include <QJsonObject>

#include <Blocks/PolyRamp.h>
#include <Midi/MidiPhysicalOutput.h>

namespace Data
{
   class Core;
}

namespace RampDevice
{
   class VCV : public QObject, public Remember::Root
   {
      Q_OBJECT
   public:
      VCV(QObject* parent);

   public:
      QJsonObject compileRamps() const;

      void pushToServer();
      void connectToServer();

   private:
      friend class Data::Core;
      using PolyRampArray_ = Remember::RefArray<PolyRamp, 16>;

   private:
      PolyRampArray_ polyRamps;
      Midi::Physical::Output output;
   };
} // namespace RampDevice

#endif // NOT RampDeviceVCVH
