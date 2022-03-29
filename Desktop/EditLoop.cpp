#include "EditLoop.h"

Edit::Loop::Loop(PolyRampWidget* polyRampWidget, MainWidget* mainWidget)
   : Abstract(polyRampWidget, mainWidget)
   , loopCheck(nullptr)
{
   loopCheck = new QCheckBox(this);
   setPayload(loopCheck, "Loop");
}

void Edit::Loop::execute(Graph* graph)
{
   const bool loop = loopCheck->isChecked();
   graph->setLooping(loop);
}
