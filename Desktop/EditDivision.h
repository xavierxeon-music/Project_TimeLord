#ifndef EditDivisionH
#define EditDivisionH

#include "EditAbstract.h"

#include <QComboBox>

namespace Edit
{
   class Division : public Abstract
   {
   public:
      Division(PolyRampWidget* polyRampWidget, MainWidget* mainWidget);

   private:
      void execute(PolyRamp* polyRamp) override;

   private:
      QComboBox* divisionCombo;
   };
} // namespace Edit

#endif // NOT EditDivisionH
