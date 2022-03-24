#include "PortWidget.h"

#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "MainWidget.h"

PortWidget::PortWidget(MainWidget* mainWidget, PortModel* portModel)
   : QWidget(mainWidget)
   , DataCore(mainWidget)
   , portModel(portModel)
{
   setMinimumWidth(150);

   QToolBar* toolBar = new QToolBar(this);
   toolBar->setIconSize(QSize(24, 24));
   toolBar->addAction(QIcon(":/LoadFromFile.svg"), "Load From File", mainWidget, &MainWidget::slotLoadFromFile);
   toolBar->addAction(QIcon(":/SaveToFile.svg"), "Save To File", mainWidget, &MainWidget::slotSaveToFile);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/LoadFromDaisy.svg"), "Load From Daisy", mainWidget, &MainWidget::slotStartLoadFromdDaisy);
   toolBar->addAction(QIcon(":/SaveToDaisy.svg"), "Save To Daisy", mainWidget, &MainWidget::slotSaveToDaisy);

   QTreeView* portTreeView = new QTreeView(this);
   portTreeView->setModel(portModel);
   portTreeView->setHeaderHidden(true);
   portTreeView->setRootIsDecorated(false);
   connect(portTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &PortWidget::slotCurrentSelectionChanged);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setSpacing(0);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(portTreeView);
}

void PortWidget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   QStandardItem* item = portModel->itemFromIndex(current);
   const QVariant itemData = item->data();
   if (itemData.isNull())
      return;

   const Provider provider = itemData.value<Provider>();
   emit signalPortChanged(provider, current.row());
}
