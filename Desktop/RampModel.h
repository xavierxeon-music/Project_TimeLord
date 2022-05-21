#ifndef RampModelH
#define RampModelH

#include "Core.h"
#include <QStandardItemModel>

namespace Ramp
{
   class Model : public QStandardItemModel, public Core::Interface
   {
      Q_OBJECT
   public:
      Model(QObject* parent);

   private:
      struct Items
      {
         QStandardItem* nameItem;
         QStandardItem* lengthItem;
         QStandardItem* stepSizeItem;
         QStandardItem* timeItem;
         QStandardItem* loopItem;
         QStandardItem* countItem;
      };
      friend class Widget;

   private:
      Items create(const Core::Identifier& identifier);
      Items find(const int& row);
      void rebuildModel(Core::Identifier identifier) override;
      void modelHasChanged(Core::Identifier identifier) override;
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
   };
} // namespace Ramp

#endif // RampModelH
