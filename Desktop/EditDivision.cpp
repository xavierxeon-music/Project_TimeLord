#include "EditDivision.h"

#include "DivisionModel.h"

Edit::Division::Division(PolyRampWidget* polyRampWidget, MainWidget* mainWidget)
   : Abstract(polyRampWidget, mainWidget)
   , divisionCombo(nullptr)
{
   divisionCombo = new QComboBox(this);
   divisionCombo->setModel(new DivisionModel(this));

   setPayload(divisionCombo, "Division");
}

void Edit::Division::execute(PolyRamp* polyRamp)
{
   const QVariant data = divisionCombo->currentData(Model::Role::Data);
   const Tempo::Division stepSize = data.value<Tempo::Division>();

   polyRamp->setStepSize(stepSize);
}
