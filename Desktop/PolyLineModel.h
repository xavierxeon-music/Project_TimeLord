#ifndef PolyLineModelH
#define PolyLineModelH

#include "DataCore.h"
#include <QStandardItemModel>

namespace PolyLine
{
   class Model : public QStandardItemModel, public Data::Core
   {
      Q_OBJECT
   public:
      Model(QObject* parent);

   public:
      void rebuildModel(const Data::Identifier& identifier) override;

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
