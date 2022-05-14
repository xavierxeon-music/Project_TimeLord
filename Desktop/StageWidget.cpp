#include "StageWidget.h"

#include <QAction>
#include <QTreeView>

#include "DelegateSpinBox.h"
#include "MainWidget.h"
#include "StageModel.h"

Stage::Widget::Widget(MainWidget* mainWidget)
   : Abstract::Widget(mainWidget)
   , stageModel(nullptr)
   , identifier()
   , selectedStageIndex(0)
{
   QAction* lockAction = toolBar->addAction(QIcon(":/Lock.svg"), "Lock Graph Size", this, &Widget::slotLockGraphSize);
   lockAction->setCheckable(true);
   lockAction->setChecked(getLockGraphSize());

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Add.svg"), "Insert Point", this, &Widget::slotInsertPoint);
   toolBar->addAction(QIcon(":/Remove.svg"), "Remove  Point", this, &Widget::slotRemovePoint);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/MoveUp.svg"), "Move Back", this, &Widget::slotMoveBack);
   toolBar->addAction(QIcon(":/MoveDown.svg"), "Move Forward", this, &Widget::slotMoveForward);

   stageModel = new Stage::Model(this);
   QTreeView* staggeTreeView = addTreeView(stageModel);
   staggeTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this));
   staggeTreeView->setItemDelegateForColumn(2, new Delegate::SpinBox(this));
   staggeTreeView->setItemDelegateForColumn(3, new Delegate::SpinBox(this));
}

void Stage::Widget::slotInsertPoint()
{
   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   polyRamp->addStage(selectedStageIndex);

   stageModel->rebuildModel(identifier);
   setSelection(selectedStageIndex);
}

void Stage::Widget::slotRemovePoint()
{
   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   polyRamp->removeStage(selectedStageIndex);

   stageModel->rebuildModel(identifier);
}

void Stage::Widget::slotMoveBack()
{
   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   if (0 == selectedStageIndex)
      return;

   polyRamp->moveStage(selectedStageIndex, selectedStageIndex - 1);

   stageModel->rebuildModel(identifier);
   setSelection(selectedStageIndex - 1);
}

void Stage::Widget::slotMoveForward()
{
   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   if (selectedStageIndex + 1 > polyRamp->getStageCount())
      return;

   polyRamp->moveStage(selectedStageIndex, selectedStageIndex + 1);

   stageModel->rebuildModel(identifier);
   setSelection(selectedStageIndex + 1);
}

void Stage::Widget::slotLockGraphSize()
{
   toggleLockGraphSize();
}

void Stage::Widget::selectionChanged(Core::Identifier newIdentifier)
{
   identifier = newIdentifier;

   stageModel->rebuildModel(identifier);
   selectedStageIndex = 0;
}
