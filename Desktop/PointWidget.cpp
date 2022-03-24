#include "PointWidget.h"

#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "MainWidget.h"

PointWidget::PointWidget(MainWidget* mainWidget, PointModel* pointModel)
   : QWidget(mainWidget)
   , DataCore(mainWidget)
{
   QToolBar* toolBar = new QToolBar(this);
   toolBar->setIconSize(QSize(24, 24));
   toolBar->addAction(QIcon(":/Add.svg"), "Insert Point", pointModel, &PointModel::slotInsertPoint);
   toolBar->addAction(QIcon(":/Remove.svg"), "Remove  Point", pointModel, &PointModel::slotRemovePoint);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Back.svg"), "Move Back", pointModel, &PointModel::slotMoveBack);
   toolBar->addAction(QIcon(":/Forward.svg"), "Move Forward", pointModel, &PointModel::slotMoveForward);

   connect(this, &PointWidget::signalPointSelected, pointModel, &PointModel::slotPointSelected);

   QTreeView* pointsTreeView = new QTreeView(this);
   pointsTreeView->setModel(pointModel);
   pointsTreeView->setHeaderHidden(true);
   pointsTreeView->setRootIsDecorated(false);
   connect(pointsTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &PointWidget::slotCurrentSelectionChanged);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setSpacing(0);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(pointsTreeView);
}

void PointWidget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   emit signalPointSelected(current.row());
}
