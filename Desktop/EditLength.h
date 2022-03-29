#ifndef EditLengthH
#define EditLengthH

#include "AbstractEdit.h"

#include <QSpinBox>

namespace Edit
{
   class Length : public Abstract::Edit
   {
   public:
      Length(Ramp::Widget* polyRampWidget, MainWidget* mainWidget);

   private:
      void execute(PolyRamp* polyRamp) override;

   private:
      QSpinBox* lengthSpin;
   };
} // namespace Edit

#endif // NOT EditLengthH
