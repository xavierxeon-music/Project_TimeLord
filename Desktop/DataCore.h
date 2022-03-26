#ifndef DataCoreH
#define DataCoreH

#include <Blocks/Graph.h>

class MainWidget;

namespace Model
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
      DaisyPatch,
      GraphAudioDevice
   };

   Q_ENUM_NS(Provider)
} // namespace Model

// all graph data access and manipulation should happen via this class
class DataCore
{
public:
   using PoviderNameMap = QMap<Model::Provider, QString>;

public:
   DataCore(MainWidget* mainWidget);

protected:
   PoviderNameMap getProviderNames() const;
   Graph* getGraph(const Model::Provider& provider, const uint8_t& index);

protected:
   MainWidget* mainWidget;

private:
   static const PoviderNameMap providerNameMap;
};

#endif // DataCoreH
