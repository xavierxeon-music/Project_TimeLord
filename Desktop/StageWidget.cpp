#include "StageWidget.h"

#include <QAction>
#include <QTreeView>

#include "DelegateSpinBox.h"
#include "MainWidget.h"
#include "StageModel.h"

Stage::Widget::Widget(MainWidget* mainWidget)
   : Abstract::Widget(mainWidget)
   , stageModel(nullptr)
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
   PolyRamp* polyRamp = getPolyRamp(selectionIdentifier);
   if (!polyRamp)
      return;

   polyRamp->addStage(selectionIdentifier.stageIndex);

   stageModel->rebuildModel(selectionIdentifier);
   setModified();

   setSelection(selectionIdentifier.stageIndex);
}

void Stage::Widget::slotRemovePoint()
{
   PolyRamp* polyRamp = getPolyRamp(selectionIdentifier);
   if (!polyRamp)
      return;

   polyRamp->removeStage(selectionIdentifier.stageIndex);

   stageModel->rebuildModel(selectionIdentifier);
   setModified();
}

void Stage::Widget::slotMoveBack()
{
   PolyRamp* polyRamp = getPolyRamp(selectionIdentifier);
   if (!polyRamp)
      return;

   if (0 == selectionIdentifier.stageIndex)
      return;

   polyRamp->moveStage(selectionIdentifier.stageIndex, selectionIdentifier.stageIndex - 1);

   stageModel->rebuildModel(selectionIdentifier);
   setModified();

   setSelection(selectionIdentifier.stageIndex - 1);
}

void Stage::Widget::slotMoveForward()
{
   PolyRamp* polyRamp = getPolyRamp(selectionIdentifier);
   if (!polyRamp)
      return;

   if (selectionIdentifier.stageIndex + 1 > polyRamp->getStageCount())
      return;

   polyRamp->moveStage(selectionIdentifier.stageIndex, selectionIdentifier.stageIndex + 1);

   stageModel->rebuildModel(selectionIdentifier);
   setModified();

   setSelection(selectionIdentifier.stageIndex + 1);
}

void Stage::Widget::slotLockGraphSize()
{
   toggleLockGraphSize();
}

void Stage::Widget::selectionChanged(Core::Identifier identifier)
{
   if (identifier.bankIndex != selectionIdentifier.bankIndex || identifier.rampIndex != selectionIdentifier.rampIndex)
      stageModel->rebuildModel(identifier);

   selectionIdentifier = identifier;
}
