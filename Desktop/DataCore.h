#ifndef DataCoreH
#define DataCoreH

#include <Blocks/Graph.h>

class MainWidget;

// all graph data access and manipulation should happen via this class
class DataCore
{
public:
   enum class Provider : uint8_t
   {
      DaisyPatch,
      GraphAudioDevice
   };
   using PoviderNameMap = QMap<Provider, QString>;

public:
   DataCore(MainWidget* mainWidget);

protected:
   PoviderNameMap getProviderNames() const;
   Graph& graph(const Provider& provider, const uint8_t& index);

protected:
   MainWidget* mainWidget;

private:
   static const PoviderNameMap providerNameMap;
};

#endif // DataCoreH
