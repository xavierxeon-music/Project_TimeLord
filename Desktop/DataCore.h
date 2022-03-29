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
      StageHeight,
      StageLength
   };
   Q_ENUM_NS(Target)

   struct Role
   {
      static constexpr int Provider = Qt::UserRole + 10;
      static constexpr int GraphIndex = Qt::UserRole + 11;
      static constexpr int StageIndex = Qt::UserRole + 12;
      static constexpr int Data = Qt::UserRole + 13;   // non integer data in original format
      static constexpr int Target = Qt::UserRole + 14; // the target enum
   };

   enum class Provider : uint8_t
   {
      None,
      DaisyPatch,
      AudioDeviceGraph
   };

   Q_ENUM_NS(Provider)

   // all graph data access and manipulation should happen via this class
   class Core
   {
   public:
      using PoviderNameMap = QMap<Provider, QString>;

   public:
      Core(MainWidget* mainWidget);

   protected:
      PoviderNameMap getProviderNames() const;
      PolyRamp* getPolyRamp(const Provider& provider, const uint8_t& index);

   protected:
      MainWidget* mainWidget;
      static bool lockGraphSize;

   private:
      static const PoviderNameMap providerNameMap;
   };

} // namespace Data

#endif // DataCoreH
