#ifndef DataCoreH
#define DataCoreH

#include <TimeLord.h>

class MainWidget;

class DataCore
{
public:
   enum class Provider : uint8_t
   {
      DaisyPatch,
      GraphDevice
   };
   using PoviderNameMap = QMap<Provider, QString>;

public:
   DataCore(MainWidget* mainWidget);

protected:
   PoviderNameMap getProviderNames() const;
   Graph& graph(const Provider& provider, const uint8_t& index);

private:
   MainWidget* mainWidget;
   PoviderNameMap providerNameMap;
};

#endif // DataCoreH
