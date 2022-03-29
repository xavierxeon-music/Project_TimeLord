#include "EditLength.h"

#include <QSpinBox>

Edit::Length::Length(PolyRampWidget* polyRampWidget, MainWidget* mainWidget)
   : Abstract(polyRampWidget, mainWidget)
   , lengthSpin(nullptr)
{
   lengthSpin = new QSpinBox(this);
   lengthSpin->setFrame(false);
   lengthSpin->setMinimum(0);
   lengthSpin->setMaximum(1000);

   setPayload(lengthSpin, "Length");
}

void Edit::Length::execute(PolyRamp* polyRamp)
{
   lengthSpin->interpretText();
   const uint32_t value = lengthSpin->value();

   polyRamp->setLength(value, true);
}
