#ifndef BankModelH
#define BankModelH

#include "DataCore.h"
#include <QStandardItemModel>

namespace Bank
{
   class Model : public QStandardItemModel, public Data::Core
   {
      Q_OBJECT
   public:
      Model(QObject* parent);

   public:
      void modelHasChanged(Data::Identifier identifier) override;

   private:
      friend class Widget;
   };
} // namespace Bank

#endif // NOT BankModelH
