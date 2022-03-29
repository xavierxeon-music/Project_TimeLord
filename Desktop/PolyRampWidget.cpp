#include "GraphWidget.h"

#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "DelegateComboBox.h"
#include "DelegateSpinBox.h"
#include "DivisionModel.h"
#include "MainWidget.h"

GraphWidget::GraphWidget(MainWidget* mainWidget, GraphModel* graphModel)
   : AbstractWidget(mainWidget)
   , graphModel(graphModel)
   , editStack(nullptr)
   , lengthEdit(nullptr)
   , divisionEdit(nullptr)
   , loopEdit(nullptr)
   , selectedProvider(Model::Provider::None)
   , selectedGraphIndex(0)
{
   setMinimumWidth(150);

   toolBar->addAction(QIcon(":/LoadFromFile.svg"), "Load From File", mainWidget, &MainWidget::slotLoadFromFile);
   QAction* saveFileAction = toolBar->addAction(QIcon(":/SaveToFile.svg"), "Save To File", mainWidget, &MainWidget::slotSaveToFile);
   saveFileAction->setShortcut(QKeySequence(QKeySequence::Save));
   toolBar->addAction(QIcon(":/SaveNewFile.svg"), "Save To New File", mainWidget, &MainWidget::slotSaveNewFile);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/SaveToDaisy.svg"), "Save To Daisy", mainWidget, &MainWidget::slotSaveToDaisy);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Trim.svg"), "Trim Selected Graph", this, &GraphWidget::slotTrimCurrentGraph);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Length.svg"), "Set Length For All Graphs", this, &GraphWidget::slotSetLengthAllGraphs);
   toolBar->addAction(QIcon(":/Division.svg"), "Set Division For All Graphs", this, &GraphWidget::slotSetDivisionAllGraphs);
   toolBar->addAction(QIcon(":/Loop.svg"), "Set Loop For All Graphs", this, &GraphWidget::slotSetLoopAllGraphs);

   editStack = new QStackedWidget(this);
   editStack->hide();
   editStack->setFixedHeight(30);

   lengthEdit = new GraphEdit::Length(this, mainWidget);
   editStack->addWidget(lengthEdit);

   divisionEdit = new GraphEdit::Division(this, mainWidget);
   editStack->addWidget(divisionEdit);

   loopEdit = new GraphEdit::Loop(this, mainWidget);
   editStack->addWidget(loopEdit);

   addPayload(editStack);

   QTreeView* graphTreeView = new QTreeView(this);
   graphTreeView->setModel(graphModel);
   graphTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   graphTreeView->setItemDelegateForColumn(2, new Delegate::ComboBox(this, mainWidget, new DivisionModel(this)));
   graphTreeView->setRootIsDecorated(false);
   connect(graphTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &GraphWidget::slotCurrentSelectionChanged);

   addPayload(graphTreeView);
}

void GraphWidget::hideEditStack()
{
   editStack->hide();
}

void GraphWidget::slotTrimCurrentGraph()
{
   Graph* graph = getGraph(selectedProvider, selectedGraphIndex);
   if (!graph)
      return;

   graph->trimLength();

   graphModel->slotGraphLengthChanged(selectedProvider, selectedGraphIndex);
}

void GraphWidget::slotSetLengthAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(lengthEdit);
}

void GraphWidget::slotSetDivisionAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(divisionEdit);
}

void GraphWidget::slotSetLoopAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(loopEdit);
}

void GraphWidget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   QStandardItem* item = graphModel->itemFromIndex(current);
   const QVariant itemDataProvider = item->data(Model::Role::Provider);
   if (itemDataProvider.isNull())
      return;

   selectedProvider = itemDataProvider.value<Model::Provider>();
   selectedGraphIndex = item->data(Model::Role::GraphIndex).toInt();

   emit signalGraphSelected(selectedProvider, selectedGraphIndex);
}
