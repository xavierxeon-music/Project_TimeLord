#include "StageWidget.h"

#include <QAction>
#include <QTreeView>

#include "DelegateSpinBox.h"
#include "MainWidget.h"

Stage::Widget::Widget(MainWidget* mainWidget, Model* stageModel)
   : Abstract::Widget(mainWidget)
   , stageModel(stageModel)
   , selectionModel(nullptr)
   , provider(Data::Provider::None)
   , graphIndex(0)
   , selectedStageIndex(0)
{
   QAction* lockAction = toolBar->addAction(QIcon(":/Lock.svg"), "Lock Graph Size", this, &Widget::slotLockGraphSize);
   lockAction->setCheckable(true);
   lockAction->setChecked(lockGraphSize);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Add.svg"), "Insert Point", this, &Widget::slotInsertPoint);
   toolBar->addAction(QIcon(":/Remove.svg"), "Remove  Point", this, &Widget::slotRemovePoint);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/MoveUp.svg"), "Move Back", this, &Widget::slotMoveBack);
   toolBar->addAction(QIcon(":/MoveDown.svg"), "Move Forward", this, &Widget::slotMoveForward);

   QTreeView* staggeTreeView = new QTreeView(this);
   staggeTreeView->setModel(stageModel);
   staggeTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   staggeTreeView->setItemDelegateForColumn(2, new Delegate::SpinBox(this, mainWidget));
   staggeTreeView->setRootIsDecorated(false);

   selectionModel = staggeTreeView->selectionModel();
   connect(selectionModel, &QItemSelectionModel::currentChanged, this, &Widget::slotCurrentSelectionChanged);

   addPayload(staggeTreeView);
}

void Stage::Widget::slotGraphSelected(const Data::Provider& newProvider, const uint8_t& newGraphIndex)
{
   provider = newProvider;
   graphIndex = newGraphIndex;

   stageModel->rebuild(provider, graphIndex, false);
   selectedStageIndex = 0;
}

void Stage::Widget::slotInsertPoint()
{
   PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
   if (!polyRamp)
      return;

   polyRamp->addStage(selectedStageIndex);

   stageModel->rebuild(provider, graphIndex, true);
   setSelection(selectedStageIndex);
}

void Stage::Widget::slotRemovePoint()
{
   PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
   if (!polyRamp)
      return;

   polyRamp->removeStage(selectedStageIndex);

   stageModel->rebuild(provider, graphIndex, true);
}

void Stage::Widget::slotMoveBack()
{
   PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
   if (!polyRamp)
      return;

   if (0 == selectedStageIndex)
      return;

   polyRamp->moveStage(selectedStageIndex, selectedStageIndex - 1);

   stageModel->rebuild(provider, graphIndex, false);
   setSelection(selectedStageIndex - 1);
}

void Stage::Widget::slotMoveForward()
{
   PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
   if (!polyRamp)
      return;

   if (selectedStageIndex + 1 > polyRamp->stageCount())
      return;

   polyRamp->moveStage(selectedStageIndex, selectedStageIndex + 1);

   stageModel->rebuild(provider, graphIndex, false);
   setSelection(selectedStageIndex + 1);
}

void Stage::Widget::slotLockGraphSize()
{
   lockGraphSize = !lockGraphSize;
}

void Stage::Widget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);
   selectedStageIndex = current.row();
}

void Stage::Widget::setSelection(const uint& stageIndex)
{
   const QModelIndex modelIndexLeft = stageModel->index(stageIndex, 0);
   const QModelIndex modelIndexRight = stageModel->index(stageIndex, stageModel->columnCount() - 1);

   selectionModel->select(QItemSelection(modelIndexLeft, modelIndexRight), QItemSelectionModel::SelectCurrent);

   selectedStageIndex = stageIndex;
}
