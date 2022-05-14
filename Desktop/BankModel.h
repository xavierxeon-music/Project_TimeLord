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

   public:
      void modelHasChanged(Core::Identifier identifier) override;

   private:
      void rebuildModel(Core::Identifier identifier = Core::Identifier()) override;

   private:
      friend class Widget;
   };
} // namespace Bank

#endif // NOT BankModelH
