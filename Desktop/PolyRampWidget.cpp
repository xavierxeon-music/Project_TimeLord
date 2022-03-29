#include "PolyRampWidget.h"

#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "DelegateComboBox.h"
#include "DelegateSpinBox.h"
#include "DivisionModel.h"
#include "MainWidget.h"

PolyRampWidget::PolyRampWidget(MainWidget* mainWidget, PolyRampModel* polyRampModel)
   : AbstractWidget(mainWidget)
   , polyRampModel(polyRampModel)
   , editStack(nullptr)
   , lengthEdit(nullptr)
   , divisionEdit(nullptr)
   , loopEdit(nullptr)
   , selectedProvider(Model::Provider::None)
   , selectedGraphIndex(0)
{
   setMinimumWidth(150);

   toolBar->addAction(QIcon(":/Trim.svg"), "Trim Selected Graph", this, &PolyRampWidget::slotTrimCurrentGraph);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Length.svg"), "Set Length For All Graphs", this, &PolyRampWidget::slotSetLengthAllGraphs);
   toolBar->addAction(QIcon(":/Division.svg"), "Set Division For All Graphs", this, &PolyRampWidget::slotSetDivisionAllGraphs);
   toolBar->addAction(QIcon(":/Loop.svg"), "Set Loop For All Graphs", this, &PolyRampWidget::slotSetLoopAllGraphs);

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

   QTreeView* graphTreeView = new QTreeView(this);
   graphTreeView->setModel(polyRampModel);
   graphTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   graphTreeView->setItemDelegateForColumn(2, new Delegate::ComboBox(this, mainWidget, new DivisionModel(this)));
   graphTreeView->setRootIsDecorated(false);
   connect(graphTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &PolyRampWidget::slotCurrentSelectionChanged);

   addPayload(graphTreeView);
}

void PolyRampWidget::hideEditStack()
{
   editStack->hide();
}

void PolyRampWidget::slotTrimCurrentGraph()
{
   PolyRamp* polyRamp = getPolyRamp(selectedProvider, selectedGraphIndex);
   if (!polyRamp)
      return;

   polyRamp->trimLength();

   polyRampModel->slotGraphLengthChanged(selectedProvider, selectedGraphIndex);
}

void PolyRampWidget::slotSetLengthAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(lengthEdit);
}

void PolyRampWidget::slotSetDivisionAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(divisionEdit);
}

void PolyRampWidget::slotSetLoopAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(loopEdit);
}

void PolyRampWidget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   QStandardItem* item = polyRampModel->itemFromIndex(current);
   const QVariant itemDataProvider = item->data(Model::Role::Provider);
   if (itemDataProvider.isNull())
      return;

   selectedProvider = itemDataProvider.value<Model::Provider>();
   selectedGraphIndex = item->data(Model::Role::GraphIndex).toInt();

   emit signalGraphSelected(selectedProvider, selectedGraphIndex);
}
