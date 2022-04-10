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

   private:
      struct Items
      {
         QStandardItem* typeItem;
         QStandardItem* posItem;
         QStandardItem* endHeightItem;
         QStandardItem* noteItem;
      };

      friend class Widget;

   private:
      Items create(const Data::Identifier& identifier);
      Items find(const int& row);
      void rebuildModel(Data::Identifier identifier) override;
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
   };
} // namespace PolyLine

#endif // NOT PolyLineModelH
