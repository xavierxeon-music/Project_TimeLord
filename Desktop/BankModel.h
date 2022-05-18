#ifndef BankModelH
#define BankModelH

#include "Core.h"
#include <QStandardItemModel>

namespace Bank
{
   class Model : public QStandardItemModel, public Core::Interface
   {
      Q_OBJECT
   public:
      Model(QObject* parent);

   private:
      friend class Widget;

   private:
      void rebuildModel(Core::Identifier identifier = Core::Identifier()) override;
      bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

   };
} // namespace Bank

#endif // NOT BankModelH
