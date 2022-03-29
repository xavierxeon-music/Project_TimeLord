#ifndef PolyLineModelH
#define PolyLineModelH

#include "DataCore.h"
#include <QStandardItemModel>

namespace PolyLine
{
   class Model : public QStandardItemModel, public Data::Core
   {
   public:
      Model(MainWidget* mainWidget);

   signals:
      void signalGraphLengthChanged(const Data::Provider& provider, const uint8_t& graphIndex);

   public:
      void rebuild(const Data::Provider provider, const uint8_t& graphIndex, bool lengthChanged);
   };
} // namespace PolyLine

#endif // NOT PolyLineModelH
