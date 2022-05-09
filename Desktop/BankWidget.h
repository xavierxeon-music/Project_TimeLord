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

   private:
      Model* bankModel;
   };
} // namespace Bank

#endif // NOT BankWidgetH
