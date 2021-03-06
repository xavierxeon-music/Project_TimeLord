#ifndef BankWidgetH
#define BankWidgetH

#include "AbstractWidget.h"

namespace Bank
{
   class Model;

   class Widget : public Abstract::Widget
   {
      Q_OBJECT

   public:
      Widget(MainWidget* mainWidget);

   private slots:
      void slotAddBank();
      void slotRemoveBank();

   private:
      Model* bankModel;
      Core::Identifier identifier;
   };
} // namespace Bank

#endif // NOT BankWidgetH
