#include "BankWidget.h"

#include <QTreeView>

#include "BankModel.h"
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

   QTreeView* bankTreeView = new QTreeView(this);
   bankTreeView->setModel(bankModel);
   //polyRampTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this));
   bankTreeView->setRootIsDecorated(false);
   connect(bankTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &Widget::slotCurrentSelectionChanged);

   addPayload(bankTreeView);
}

void Bank::Widget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   QStandardItem* item = bankModel->itemFromIndex(current);
   identifier = item->data(Core::Role::Identifier).value<Core::Identifier>();

   callOnAllInstances(&Interface::polyRampSelected, identifier);
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
