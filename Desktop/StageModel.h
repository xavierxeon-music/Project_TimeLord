#ifndef StageModelH
#define StageModelH

#include "Core.h"
#include <QStandardItemModel>

namespace Stage
{
   class Model : public QStandardItemModel, public Core::Interface
   {
      Q_OBJECT
   public:
      Model(QObject* parent);

   private:
      struct Items
      {
         QStandardItem* startPosItem;
         QStandardItem* startTimeItem;
         QStandardItem* lengthItem;
         QStandardItem* startHeigthItem;
         QStandardItem* endHeigthItem;
      };
      friend class Widget;

   private:
      Items create(const Core::Identifier& identifier);
      Items find(const int& row);
      void rebuildModel(Core::Identifier identifier) override;
      void selectionChanged(Core::Identifier identifier) override;
      void modelHasChanged(Core::Identifier identifier) override;
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
   };
} // namespace Stage

#endif // StageModelH
