#include "EditLoop.h"

Edit::Loop::Loop(PolyRampWidget* polyRampWidget, MainWidget* mainWidget)
   : Abstract(polyRampWidget, mainWidget)
   , loopCheck(nullptr)
{
   loopCheck = new QCheckBox(this);
   setPayload(loopCheck, "Loop");
}

void Edit::Loop::execute(PolyRamp* polyRamp)
{
   const bool loop = loopCheck->isChecked();
   polyRamp->setLooping(loop);
}
