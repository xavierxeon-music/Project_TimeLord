#include "BankWidget.h"

#include <QTreeView>

#include "BankModel.h"
#include "DelegateSpinBox.h"
#include "MainWidget.h"

Bank::Widget::Widget(MainWidget* mainWidget)
   : Abstract::Widget(mainWidget)
   , bankModel(nullptr)
   , identifier()
{
   setMinimumWidth(50);

   toolBar->addAction(QIcon(":/Add.svg"), "Add Bank", this, &Widget::slotAddBank);
   toolBar->addAction(QIcon(":/Remove.svg"), "Remove Bank", this, &Widget::slotRemoveBank);

   bankModel = new Model(this);
   //bankModel->rebuildModel();

   QTreeView* bankTreeView = addTreeView(bankModel);
   bankTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this));
}

void Bank::Widget::slotAddBank()
{
   addBank();
   callOnAllInstances(&Interface::rebuildModel, Core::Identifier());
}

void Bank::Widget::slotRemoveBank()
{
   removeBank();
   callOnAllInstances(&Interface::rebuildModel, Core::Identifier());
}
