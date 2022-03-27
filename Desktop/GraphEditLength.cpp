#include "GraphEditLength.h"

#include <QSpinBox>

GraphEdit::Length::Length(GraphWidget* graphWidget, MainWidget* mainWidget)
   : Abstract(graphWidget, mainWidget)
   , lengthSpin(nullptr)
{
   lengthSpin = new QSpinBox(this);
   lengthSpin->setFrame(false);
   lengthSpin->setMinimum(0);
   lengthSpin->setMaximum(1000);

   setPayload(lengthSpin, "Length");
}

void GraphEdit::Length::execute(Graph* graph)
{
   lengthSpin->interpretText();
   const uint32_t value = lengthSpin->value();

   graph->setLength(value, true);
}
