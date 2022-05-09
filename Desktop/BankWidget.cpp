#include "BankWidget.h"

#include "BankModel.h"
#include "MainWidget.h"

Bank::Widget::Widget(MainWidget* mainWidget)
   : Abstract::Widget(mainWidget)
   , bankModel(nullptr)
{
   bankModel = new Model(this);
   //bankModel->rebuildModel();
}

