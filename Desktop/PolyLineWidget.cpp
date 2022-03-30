#include "PolyLineWidget.h"

#include <QTreeView>

#include "PolyLineModel.h"

PolyLine::Widget::Widget(MainWidget* mainWidget, Model* polyLineModel)
   : Abstract::Widget(mainWidget)
   , polyLineModel(polyLineModel)
   , selectionModel(nullptr)
   , identifier()
   , selectedStageIndex(0)
{
   setMinimumWidth(150);

   toolBar->addAction(QIcon(":/Add.svg"), "Insert Point", this, &Widget::slotInsertPoint);
   toolBar->addAction(QIcon(":/Remove.svg"), "Remove  Point", this, &Widget::slotRemovePoint);

   QTreeView* polyLineTreeView = new QTreeView(this);
   polyLineTreeView->setModel(polyLineModel);
   //polyLineTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, mainWidget));
   //polyLineTreeView->setItemDelegateForColumn(2, new Delegate::ComboBox(this, mainWidget, new DivisionModel(this)));
   polyLineTreeView->setRootIsDecorated(false);
   connect(polyLineTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &Widget::slotCurrentSelectionChanged);

   addPayload(polyLineTreeView);
}

void PolyLine::Widget::slotGraphSelected(const Data::Identifier& newIdentifier)
{
   identifier = newIdentifier;

   polyLineModel->rebuild(identifier);
   selectedStageIndex = 0;
}

void PolyLine::Widget::slotInsertPoint()
{
   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   polyRamp->addStage(selectedStageIndex);

   polyLineModel->rebuild(identifier);
   setSelection(selectedStageIndex);
}

void PolyLine::Widget::slotRemovePoint()
{
   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   polyRamp->removeStage(selectedStageIndex);

   polyLineModel->rebuild(identifier);
}

void PolyLine::Widget::slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
   Q_UNUSED(previous);
   selectedStageIndex = current.row();
}

void PolyLine::Widget::setSelection(const uint& stageIndex)
{
   const QModelIndex modelIndexLeft = polyLineModel->index(stageIndex, 0);
   const QModelIndex modelIndexRight = polyLineModel->index(stageIndex, polyLineModel->columnCount() - 1);

   selectionModel->select(QItemSelection(modelIndexLeft, modelIndexRight), QItemSelectionModel::SelectCurrent);

   selectedStageIndex = stageIndex;
}
