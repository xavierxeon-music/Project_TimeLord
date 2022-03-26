#include "StageWidget.h"

#include <QAction>

#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "DelegateSpinBox.h"
#include "MainWidget.h"

StageWidget::StageWidget(MainWidget* mainWidget, QToolBar* toolBar, StageModel* stageModel)
   : AbstractWidget(mainWidget, toolBar, "points")
   , stageModel(stageModel)
{
   toolBar->addAction(QIcon(":/Add.svg"), "Insert Point", stageModel, &StageModel::slotInsertPoint);
   toolBar->addAction(QIcon(":/Remove.svg"), "Remove  Point", stageModel, &StageModel::slotRemovePoint);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Back.svg"), "Move Back", stageModel, &StageModel::slotMoveBack);
   toolBar->addAction(QIcon(":/Forward.svg"), "Move Forward", stageModel, &StageModel::slotMoveForward);

   connect(this, &StageWidget::signalPointSelected, stageModel, &StageModel::slotPointSelected);

   QTreeView* pointsTreeView = new QTreeView(this);
   pointsTreeView->setModel(stageModel);
   pointsTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   pointsTreeView->setItemDelegateForColumn(2, new Delegate::SpinBox(this, mainWidget));

   pointsTreeView->setRootIsDecorated(false);
   connect(pointsTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &StageWidget::slotCurrentSelectionChanged);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setSpacing(0);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(pointsTreeView);
}

void StageWidget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   emit signalPointSelected(current.row());
}
