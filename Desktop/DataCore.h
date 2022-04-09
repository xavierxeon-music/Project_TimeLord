#ifndef DataCoreH
#define DataCoreH

#include <Blocks/PolyRamp.h>

class MainWidget;

namespace RampDevice
{
   class Raspi;
};

namespace Data
{
   Q_NAMESPACE

   enum class Target : uint8_t
   {
      None,
      GraphLength,
      GraphStepSize,
      GraphLoop,
      GraphStageCount,
      StageStartHeight,
      StageEndHeight,
      StageLength,
      StageType
   };
   Q_ENUM_NS(Target)

   struct Identifier
   {
      uint8_t rampIndex;
      uint8_t stageIndex;

      Identifier(const uint8_t& rampIndex = 0, const uint8_t& stageIndex = 255);
      bool hasStage() const;
   };

   struct Role
   {
      static constexpr int Identifier = Qt::UserRole + 11; // the identifier
      static constexpr int Target = Qt::UserRole + 12;     // the target enum
      static constexpr int Data = Qt::UserRole + 13;       // non integer data in original format
   };

   enum class Type : uint8_t
   {
      Anchor,
      Rise,
      Fall,
      Stable,
      Step
   };
   Q_ENUM_NS(Type)

   // all graph data access and manipulation should happen via this class
   class Core
   {      
   public:
      Core();

   public:
      virtual void modelHasChanged(const Identifier& identifier);
      virtual void polyRampSelected(const Identifier& identifier);
      virtual void rebuildModel(const Identifier& identifier);

   protected:
      using InstanceFunctionPointer = void (Core::*)(const Identifier&);

   protected:
      PolyRamp* getPolyRamp(const Identifier& identifier); // ignores stage index

      void callOnAllInstances(const Identifier& identifier, InstanceFunctionPointer instanceFunctionPointer);

      void setLockGraphSize(bool locked);
      void toggleLockGraphSize();
      bool getLockGraphSize() const;

   private:
      friend class ::MainWidget;

   private:
      static void init(RampDevice::Raspi* raspiDevice);

   private:
      static bool lockGraphSize;
      static RampDevice::Raspi* raspiDevice;
      static QList<Core*> instanceList;
   };

} // namespace Data

#endif // DataCoreH
