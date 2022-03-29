#ifndef EditLoopH
#define EditLoopH

#include "AbstractEdit.h"

#include <QCheckBox>

namespace Edit
{
   class Loop : public Abstract::Edit
   {
   public:
      Loop(Ramp::Widget* polyRampWidget, MainWidget* mainWidget);

   private:
      void execute(PolyRamp* polyRamp) override;

   private:
      QCheckBox* loopCheck;
   };
} // namespace Edit

#endif // NOT EditLoopH
