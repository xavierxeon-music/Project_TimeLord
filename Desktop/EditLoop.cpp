#include "EditLoop.h"

Edit::Loop::Loop(Ramp::Widget* polyRampWidget, MainWidget* mainWidget)
   : Abstract::Edit(polyRampWidget, mainWidget)
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
