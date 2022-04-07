#include "EditDivision.h"

#include "DivisionModel.h"

Edit::Division::Division(Ramp::Widget* polyRampWidget)
   : Abstract::Edit(polyRampWidget)
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
