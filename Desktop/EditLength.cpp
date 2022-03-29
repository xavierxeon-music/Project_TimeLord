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

void Edit::Length::execute(Graph* graph)
{
   lengthSpin->interpretText();
   const uint32_t value = lengthSpin->value();

   graph->setLength(value, true);
}
