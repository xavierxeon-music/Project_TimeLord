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
      void signalRampChanged(const Data::Identifier& identifier);

   public:
      void rebuild(const Data::Identifier& identifier);

   private:
      struct Items
      {
         QStandardItem* startPosItem;
         QStandardItem* typeItem;
         QStandardItem* endHeightItem;
         QStandardItem* noteItem;

         Items(Model* model, const Data::Identifier& identifier);
      };
   };
} // namespace PolyLine

#endif // NOT PolyLineModelH
