#ifndef DataCoreH
#define DataCoreH

#include <Blocks/PolyRamp.h>

class MainWidget;

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

   struct Role
   {
      static constexpr int RampIndex = Qt::UserRole + 11;
      static constexpr int StageIndex = Qt::UserRole + 12;
      static constexpr int Data = Qt::UserRole + 13;   // non integer data in original format
      static constexpr int Target = Qt::UserRole + 14; // the target enum
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

   struct Identifier
   {
      uint8_t rampIndex;
      uint8_t stageIndex;

      Identifier(const uint8_t& rampIndex = 0, const uint8_t& stageIndex = 0);
   };

   // all graph data access and manipulation should happen via this class
   class Core
   {

   public:
      Core(MainWidget* mainWidget);

   protected:
      PolyRamp* getPolyRamp(const Identifier& identifier);

   protected:
      MainWidget* mainWidget;
      static bool lockGraphSize;

   };

} // namespace Data

#endif // DataCoreH
