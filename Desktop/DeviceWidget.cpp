#include "DeviceWidget.h"

#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "ComboBoxDelegate.h"
#include "MainWidget.h"
#include "SpinBoxDelegate.h"

DeviceWidget::DeviceWidget(MainWidget* mainWidget, QToolBar* toolBar, DeviceModel* deviceModel)
   : AbstractWidget(mainWidget, toolBar, "devices")
   , deviceModel(deviceModel)
{
   setMinimumWidth(150);

   toolBar->addAction(QIcon(":/LoadFromFile.svg"), "Load From File", mainWidget, &MainWidget::slotLoadFromFile);
   toolBar->addAction(QIcon(":/SaveToFile.svg"), "Save To File", mainWidget, &MainWidget::slotSaveToFile);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/SaveToDaisy.svg"), "Save To Daisy", mainWidget, &MainWidget::slotSaveToDaisy);

   QTreeView* portTreeView = new QTreeView(this);
   portTreeView->setModel(deviceModel);
   portTreeView->setItemDelegateForColumn(1, new SpinBoxDelegate(this, mainWidget, Model::Target::GraphLength));
   portTreeView->setItemDelegateForColumn(2, new ComboBoxDelegate(this, mainWidget, Model::Target::GraphStepSize));
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
   const QVariant itemDataProvider = item->data(Model::Role::Provider);
   if (itemDataProvider.isNull())
      return;

   const Model::Provider provider = itemDataProvider.value<Model::Provider>();

   const int graphIndex = item->data(Model::Role::GraphIndex).toInt();
   emit signalPortChanged(provider, graphIndex);
}
