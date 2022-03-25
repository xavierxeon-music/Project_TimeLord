#include "DeviceWidget.h"

#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "MainWidget.h"

DeviceWidget::DeviceWidget(MainWidget* mainWidget, QToolBar* toolBar, DeviceModel* deviceModel)
   : AbstractWidget(mainWidget, toolBar, "devices")
   , deviceModel(deviceModel)
{
   setMinimumWidth(150);

   toolBar->addAction(QIcon(":/NewFile.svg"), "NewFile", mainWidget, &MainWidget::slotNewFile);
   toolBar->addAction(QIcon(":/LoadFromFile.svg"), "Load From File", mainWidget, &MainWidget::slotLoadFromFile);
   toolBar->addAction(QIcon(":/SaveToFile.svg"), "Save To File", mainWidget, &MainWidget::slotSaveToFile);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/SaveToDaisy.svg"), "Save To Daisy", mainWidget, &MainWidget::slotSaveToDaisy);

   QTreeView* portTreeView = new QTreeView(this);
   portTreeView->setModel(deviceModel);
   portTreeView->setHeaderHidden(true);
   portTreeView->setRootIsDecorated(false);
   connect(portTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &DeviceWidget::slotCurrentSelectionChanged);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setSpacing(0);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(portTreeView);
}

void DeviceWidget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   QStandardItem* item = deviceModel->itemFromIndex(current);
   const QVariant itemData = item->data();
   if (itemData.isNull())
      return;

   const Provider provider = itemData.value<Provider>();
   emit signalPortChanged(provider, current.row());
}
