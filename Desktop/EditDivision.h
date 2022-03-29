#ifndef EditDivisionH
#define EditDivisionH

#include "AbstractEdit.h"

#include <QComboBox>

namespace Edit
{
   class Division : public Abstract::Edit
   {
   public:
      Division(Ramp::Widget* polyRampWidget, MainWidget* mainWidget);

   private:
      void execute(PolyRamp* polyRamp) override;

   private:
      QComboBox* divisionCombo;
   };
} // namespace Edit

#endif // NOT EditDivisionH
