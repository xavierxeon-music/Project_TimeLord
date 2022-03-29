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
      void execute(Graph* graph) override;

   private:
      QComboBox* divisionCombo;
   };
} // namespace Edit

#endif // NOT EditDivisionH
