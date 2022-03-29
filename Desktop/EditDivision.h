#ifndef GraphEditDivisionH
#define GraphEditDivisionH

#include "GraphEditAbstract.h"

#include <QComboBox>

namespace GraphEdit
{
   class Division : public Abstract
   {
   public:
      Division(GraphWidget* graphWidget, MainWidget* mainWidget);

   private:
      void execute(Graph* graph) override;

   private:
      QComboBox* divisionCombo;
   };
} // namespace GraphEdit

#endif // NOT GraphEditDivisionH
