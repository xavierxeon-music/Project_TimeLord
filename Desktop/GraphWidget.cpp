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
{
   setMinimumWidth(150);

   toolBar->addAction(QIcon(":/LoadFromFile.svg"), "Load From File", mainWidget, &MainWidget::slotLoadFromFile);
   toolBar->addAction(QIcon(":/SaveToFile.svg"), "Save To File", mainWidget, &MainWidget::slotSaveToFile);
   toolBar->addAction(QIcon(":/SaveNewFile.svg"), "Save To New File", mainWidget, &MainWidget::slotSaveNewFile);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/SaveToDaisy.svg"), "Save To Daisy", mainWidget, &MainWidget::slotSaveToDaisy);

   QTreeView* graphTreeView = new QTreeView(this);
   graphTreeView->setModel(graphModel);
   graphTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   graphTreeView->setItemDelegateForColumn(2, new Delegate::ComboBox(this, mainWidget, new DivisionModel(this)));
   graphTreeView->setRootIsDecorated(false);
   connect(graphTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &GraphWidget::slotCurrentSelectionChanged);

   addPayload(graphTreeView);
}

void GraphWidget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   QStandardItem* item = graphModel->itemFromIndex(current);
   const QVariant itemDataProvider = item->data(Model::Role::Provider);
   if (itemDataProvider.isNull())
      return;

   const Model::Provider provider = itemDataProvider.value<Model::Provider>();

   const int graphIndex = item->data(Model::Role::GraphIndex).toInt();
   emit signalPortChanged(provider, graphIndex);
}
