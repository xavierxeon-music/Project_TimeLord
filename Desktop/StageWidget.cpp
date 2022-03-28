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
   , selectionModel(nullptr)
   , provider(Model::Provider::None)
   , graphIndex(0)
   , selectedStageIndex(0)
{
   QAction* lockAction = toolBar->addAction(QIcon(":/Lock.svg"), "Lock Graph Size", this, &StageWidget::slotLockGraphSize);
   lockAction->setCheckable(true);
   lockAction->setChecked(lockGraphSize);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Add.svg"), "Insert Point", this, &StageWidget::slotInsertPoint);
   toolBar->addAction(QIcon(":/Remove.svg"), "Remove  Point", this, &StageWidget::slotRemovePoint);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/MoveUp.svg"), "Move Back", this, &StageWidget::slotMoveBack);
   toolBar->addAction(QIcon(":/MoveDown.svg"), "Move Forward", this, &StageWidget::slotMoveForward);

   QTreeView* staggeTreeView = new QTreeView(this);
   staggeTreeView->setModel(stageModel);
   staggeTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   staggeTreeView->setItemDelegateForColumn(2, new Delegate::SpinBox(this, mainWidget));
   staggeTreeView->setRootIsDecorated(false);

   selectionModel = staggeTreeView->selectionModel();
   connect(selectionModel, &QItemSelectionModel::currentChanged, this, &StageWidget::slotCurrentSelectionChanged);

   addPayload(staggeTreeView);
}

void StageWidget::slotGraphSelected(const Model::Provider& newProvider, const uint8_t& newGraphIndex)
{
   provider = newProvider;
   graphIndex = newGraphIndex;

   stageModel->rebuild(provider, graphIndex, false);
   selectedStageIndex = 0;
}

void StageWidget::slotInsertPoint()
{
   Graph* graph = getGraph(provider, graphIndex);
   if (!graph)
      return;

   graph->addStage(selectedStageIndex);

   stageModel->rebuild(provider, graphIndex, true);
   setSelection(selectedStageIndex);
}

void StageWidget::slotRemovePoint()
{
   Graph* graph = getGraph(provider, graphIndex);
   if (!graph)
      return;

   graph->removeStage(selectedStageIndex);

   stageModel->rebuild(provider, graphIndex, true);
}

void StageWidget::slotMoveBack()
{
   Graph* graph = getGraph(provider, graphIndex);
   if (!graph)
      return;

   if (0 == selectedStageIndex)
      return;

   graph->moveStage(selectedStageIndex, selectedStageIndex - 1);

   stageModel->rebuild(provider, graphIndex, false);
   setSelection(selectedStageIndex - 1);
}

void StageWidget::slotMoveForward()
{
   Graph* graph = getGraph(provider, graphIndex);
   if (!graph)
      return;

   if (selectedStageIndex + 1 > graph->stageCount())
      return;

   graph->moveStage(selectedStageIndex, selectedStageIndex + 1);

   stageModel->rebuild(provider, graphIndex, false);
   setSelection(selectedStageIndex + 1);
}

void StageWidget::slotLockGraphSize()
{
   lockGraphSize = !lockGraphSize;
}

void StageWidget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);
   selectedStageIndex = current.row();
}

void StageWidget::setSelection(const uint& stageIndex)
{
   const QModelIndex modelIndexLeft = stageModel->index(stageIndex, 0);
   const QModelIndex modelIndexRight = stageModel->index(stageIndex, stageModel->columnCount() - 1);

   selectionModel->select(QItemSelection(modelIndexLeft, modelIndexRight), QItemSelectionModel::SelectCurrent);

   selectedStageIndex = stageIndex;
}
