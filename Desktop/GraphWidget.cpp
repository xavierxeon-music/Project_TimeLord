#include "GraphWidget.h"

#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "DelegateComboBox.h"
#include "DelegateSpinBox.h"
#include "DivisionModel.h"
#include "MainWidget.h"

GraphWidget::GraphWidget(MainWidget* mainWidget, QToolBar* toolBar, GraphModel* graphModel)
   : AbstractWidget(mainWidget, toolBar, "graph")
   , graphModel(graphModel)
{
   setMinimumWidth(150);

   toolBar->addAction(QIcon(":/LoadFromFile.svg"), "Load From File", mainWidget, &MainWidget::slotLoadFromFile);
   toolBar->addAction(QIcon(":/SaveToFile.svg"), "Save To File", mainWidget, &MainWidget::slotSaveToFile);
   toolBar->addAction(QIcon(":/SaveNewFile.svg"), "Save To New File", mainWidget, &MainWidget::slotSaveNewFile);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/SaveToDaisy.svg"), "Save To Daisy", mainWidget, &MainWidget::slotSaveToDaisy);

   QTreeView* portTreeView = new QTreeView(this);
   portTreeView->setModel(graphModel);
   portTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   portTreeView->setItemDelegateForColumn(2, new Delegate::ComboBox(this, mainWidget, new DivisionModel(this)));
   portTreeView->setRootIsDecorated(false);
   connect(portTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &GraphWidget::slotCurrentSelectionChanged);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setSpacing(0);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(portTreeView);
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
