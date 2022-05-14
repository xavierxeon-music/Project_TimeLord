#include "AbstractWidget.h"

#include <QItemSelectionModel>
#include <QStandardItemModel>
#include <QTreeView>

#include "MainWidget.h"

Abstract::Widget::Widget(MainWidget* mainWidget)
   : QWidget(mainWidget)
   , Core::Interface()
   , toolBar(nullptr)
   , treeView(nullptr)
   , selectionModel(nullptr)
   , selectionIdentifier()
   , model()
   , masterLayout(nullptr)
{
   toolBar = new QToolBar(this);
   toolBar->setIconSize(QSize(24, 24));

   masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(toolBar);
}

QTreeView* Abstract::Widget::addTreeView(QStandardItemModel* newModel)
{
   model = newModel;

   treeView = new QTreeView(this);
   treeView->setModel(model);
   treeView->setRootIsDecorated(false);

   selectionModel = treeView->selectionModel();
   connect(selectionModel, &QItemSelectionModel::currentChanged, this, &Widget::slotCurrentSelectionChanged);

   masterLayout->addWidget(treeView);

   return treeView;
}

void Abstract::Widget::addWidget(QWidget* widget)
{
   masterLayout->addWidget(widget);
}

void Abstract::Widget::setSelection(const uint& row)
{
   const QModelIndex modelIndexLeft = model->index(row, 0);
   const QModelIndex modelIndexRight = model->index(row, model->columnCount() - 1);

   selectionModel->select(QItemSelection(modelIndexLeft, modelIndexRight), QItemSelectionModel::SelectCurrent);

   slotCurrentSelectionChanged(modelIndexLeft, QModelIndex());
}

void Abstract::Widget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   QStandardItem* item = model->itemFromIndex(current);
   if (!item)
      return;

   selectionIdentifier = item->data(Core::Role::Identifier).value<Core::Identifier>();

   callOnAllInstances(&Interface::selectionChanged, selectionIdentifier);
}
