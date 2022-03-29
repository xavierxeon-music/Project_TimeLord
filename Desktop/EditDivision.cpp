#include "GraphEditDivision.h"

#include "DivisionModel.h"

GraphEdit::Division::Division(GraphWidget* graphWidget, MainWidget* mainWidget)
   : Abstract(graphWidget, mainWidget)
   , divisionCombo(nullptr)
{
   divisionCombo = new QComboBox(this);
   divisionCombo->setModel(new DivisionModel(this));

   setPayload(divisionCombo, "Division");
}

void GraphEdit::Division::execute(Graph* graph)
{
   const QVariant data = divisionCombo->currentData(Model::Role::Data);
   const Tempo::Division stepSize = data.value<Tempo::Division>();

   graph->setStepSize(stepSize);
}
