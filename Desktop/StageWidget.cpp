#include "StageWidget.h"

#include <QAction>

#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "DelegateSpinBox.h"
#include "MainWidget.h"

StageWidget::StageWidget(MainWidget* mainWidget, StageModel* stageModel)
   : AbstractWidget(mainWidget)
   , stageModel(stageModel)
{
   toolBar->addAction(QIcon(":/Add.svg"), "Insert Point", stageModel, &StageModel::slotInsertPoint);
   toolBar->addAction(QIcon(":/Remove.svg"), "Remove  Point", stageModel, &StageModel::slotRemovePoint);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Back.svg"), "Move Back", stageModel, &StageModel::slotMoveBack);
   toolBar->addAction(QIcon(":/Forward.svg"), "Move Forward", stageModel, &StageModel::slotMoveForward);

   connect(this, &StageWidget::signalPointSelected, stageModel, &StageModel::slotPointSelected);

   QTreeView* staggeTreeView = new QTreeView(this);
   staggeTreeView->setModel(stageModel);
   staggeTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   staggeTreeView->setItemDelegateForColumn(2, new Delegate::SpinBox(this, mainWidget));

   staggeTreeView->setRootIsDecorated(false);
   connect(staggeTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &StageWidget::slotCurrentSelectionChanged);

   addPayload(staggeTreeView);
}

void StageWidget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);

   emit signalPointSelected(current.row());
}
