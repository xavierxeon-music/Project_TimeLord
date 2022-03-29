#include "PolyLineWidget.h"

#include <QTreeView>

#include "PolyLineModel.h"

PolyLine::Widget::Widget(MainWidget* mainWidget, Model* polyLineModel)
   : AbstractWidget(mainWidget)
   , polyLineModel(polyLineModel)
   , selectionModel(nullptr)
   , provider(Data::Provider::None)
   , graphIndex(0)
   , selectedStageIndex(0)

{
   toolBar->addAction(QIcon(":/Add.svg"), "Insert Point", this, &Widget::slotInsertPoint);
   toolBar->addAction(QIcon(":/Remove.svg"), "Remove  Point", this, &Widget::slotRemovePoint);
}

void PolyLine::Widget::slotGraphSelected(const Data::Provider& newProvider, const uint8_t& newGraphIndex)
{
   provider = newProvider;
   graphIndex = newGraphIndex;

   polyLineModel->rebuild(provider, graphIndex, false);
   selectedStageIndex = 0;
}

void PolyLine::Widget::slotInsertPoint()
{
   PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
   if (!polyRamp)
      return;

   polyRamp->addStage(selectedStageIndex);

   polyLineModel->rebuild(provider, graphIndex, true);
   setSelection(selectedStageIndex);
}

void PolyLine::Widget::slotRemovePoint()
{
   PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
   if (!polyRamp)
      return;

   polyRamp->removeStage(selectedStageIndex);

   polyLineModel->rebuild(provider, graphIndex, true);
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