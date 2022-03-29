#include "EditDivision.h"

#include "DivisionModel.h"

Edit::Division::Division(Ramp::Widget* polyRampWidget, MainWidget* mainWidget)
   : Abstract::Edit(polyRampWidget, mainWidget)
   , divisionCombo(nullptr)
{
   divisionCombo = new QComboBox(this);
   divisionCombo->setModel(new DivisionModel(this));

   setPayload(divisionCombo, "Division");
}

void Edit::Division::execute(PolyRamp* polyRamp)
{
   const QVariant data = divisionCombo->currentData(Data::Role::Data);
   const Tempo::Division stepSize = data.value<Tempo::Division>();

   polyRamp->setStepSize(stepSize);
}
