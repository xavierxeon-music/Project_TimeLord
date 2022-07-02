#include "RampWidget.h"

#include <QAction>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

#include "DelegateComboBox.h"
#include "DelegateSpinBox.h"
#include "MainWidget.h"
#include "ModelDivision.h"
#include "RampModel.h"

Ramp::Widget::Widget(MainWidget* mainWidget)
   : Abstract::Widget(mainWidget)
   , rampModel(nullptr)
   , editStack(nullptr)
   , lengthEdit(nullptr)
   , divisionEdit(nullptr)
   , loopEdit(nullptr)
   , identifier()
{
   setMinimumWidth(150);

   toolBar->addAction(QIcon(":/Trim.svg"), "Trim Selected Graph", this, &Widget::slotTrimCurrentGraph);
   toolBar->addSeparator();

   toolBar->addAction(QIcon(":/Length.svg"), "Set Length For All Graphs", this, &Widget::slotSetLengthAllGraphs);
   toolBar->addAction(QIcon(":/Division.svg"), "Set Division For All Graphs", this, &Widget::slotSetDivisionAllGraphs);
   toolBar->addAction(QIcon(":/Loop.svg"), "Set Loop For All Graphs", this, &Widget::slotSetLoopAllGraphs);

   editStack = new QStackedWidget(this);
   editStack->hide();
   editStack->setFixedHeight(30);

   lengthEdit = new Edit::Length(this);
   editStack->addWidget(lengthEdit);

   divisionEdit = new Edit::Division(this);
   editStack->addWidget(divisionEdit);

   loopEdit = new Edit::Loop(this);
   editStack->addWidget(loopEdit);

   addWidget(editStack);

   rampModel = new Model(this);

   QTreeView* rampTreeView = addTreeView(rampModel);
   rampTreeView->setItemDelegateForColumn(1, new Delegate::SpinBox(this, 1000));
   rampTreeView->setItemDelegateForColumn(2, new Delegate::ComboBox(this, new ::Model::Division(this)));
}

void Ramp::Widget::hideEditStack()
{
   editStack->hide();
}

void Ramp::Widget::slotTrimCurrentGraph()
{
   PolyRamp* polyRamp = getPolyRamp(identifier);
   if (!polyRamp)
      return;

   polyRamp->trimLength();

   rampModel->modelHasChanged(identifier);
}

void Ramp::Widget::slotSetLengthAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(lengthEdit);
}

void Ramp::Widget::slotSetDivisionAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(divisionEdit);
}

void Ramp::Widget::slotSetLoopAllGraphs()
{
   editStack->show();
   editStack->setCurrentWidget(loopEdit);
}

void Ramp::Widget::selectionChanged(Core::Identifier identifier)
{
   if (identifier.bankIndex != selectionIdentifier.bankIndex)
      rampModel->rebuildModel(identifier);

   selectionIdentifier = identifier;
}
