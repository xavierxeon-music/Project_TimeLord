#ifndef EditLengthH
#define EditLengthH

#include "EditAbstract.h"

#include <QSpinBox>

namespace Edit
{
   class Length : public Abstract
   {
   public:
      Length(PolyRampWidget* polyRampWidget, MainWidget* mainWidget);

   private:
      void execute(PolyRamp* polyRamp) override;

   private:
      QSpinBox* lengthSpin;
   };
} // namespace Edit

#endif // NOT EditLengthH
