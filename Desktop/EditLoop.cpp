#include "EditLoop.h"

Edit::Loop::Loop(Ramp::Widget* polyRampWidget)
   : Abstract::Edit(polyRampWidget)
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
