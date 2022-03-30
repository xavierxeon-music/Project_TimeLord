#include "RampWidget.h"

#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "DelegateComboBox.h"
#include "DelegateSpinBox.h"
#include "DivisionModel.h"
#include "MainWidget.h"

Ramp::Widget::Widget(MainWidget* mainWidget, Model* polyRampModel)
   : Abstract::Widget(mainWidget)
   , polyRampModel(polyRampModel)
   , editStack(nullptr)
   , lengthEdit(nullptr)
   , divisionEdit(nullptr)
   , loopEdit(nullptr)
   , identifier()
{
   setMinimumWidth(150);

   toolBar->addAction(QIcon(":/Trim.svg"), "Trim Selected Graph", this, &Widget::slotTrimCurrentGraph);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Length.svg"), "Set Length For All Graphs", this, &Widget::slotSetLengthAllGraphs);
   toolBar->addAction(QIcon(":/Division.svg"), "Set Division For All Graphs", this, &Widget::slotSetDivisionAllGraphs);
   toolBar->addAction(QIcon(":/Loop.svg"), "Set Loop For All Graphs", this, &Widget::slotSetLoopAllGraphs);

   editStack = new QStackedWidget(this);
   editStack->hide();
   editStack->setFixedHeight(30);

   lengthEdit = new Edit::Length(this, mainWidget);
   editStack->addWidget(lengthEdit);

   divisionEdit = new Edit::Division(this, mainWidget);
   editStack->addWidget(divisionEdit);

   loopEdit = new Edit::Loop(this, mainWidget);
   editStack->addWidget(loopEdit);

   addPayload(editStack);

   QTreeView* polyRampTreeView = new QTreeView(this);
   polyRampTreeView->setModel(polyRampModel);
   polyRampTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   polyRampTreeView->setItemDelegateForColumn(2, new Delegate::ComboBox(this, mainWidget, new DivisionModel(this)));
   polyRampTreeView->setRootIsDecorated(false);
   connect(polyRampTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &Widget::slotCurrentSelectionChanged);

   addPayload(polyRampTreeView);
}

void Ramp::Widget::hideEditStack()
{
   editStack->hide();
}

void Ramp::Widget::slotTrimCurrentGraph()
{
   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   polyRamp->trimLength();

   polyRampModel->slotRampChanged(identifier);
}

void Ramp::Widget::slotSetLengthAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(lengthEdit);
}

void Ramp::Widget::slotSetDivisionAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(divisionEdit);
}

void Ramp::Widget::slotSetLoopAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(loopEdit);
}

void Ramp::Widget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   QStandardItem* item = polyRampModel->itemFromIndex(current);
   const QVariant itemDataProvider = item->data(Data::Role::Provider);
   if (itemDataProvider.isNull())
      return;

   identifier.provider = itemDataProvider.value<Data::Provider>();
   identifier.rampIndex = item->data(Data::Role::RampIndex).toInt();

   emit signalGraphSelected(identifier);
}
