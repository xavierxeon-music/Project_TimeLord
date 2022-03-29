#ifndef GraphEditLengthH
#define GraphEditLengthH

#include "GraphEditAbstract.h"

#include <QSpinBox>

namespace GraphEdit
{
   class Length : public Abstract
   {
   public:
      Length(GraphWidget* graphWidget, MainWidget* mainWidget);

   private:
      void execute(Graph* graph) override;

   private:
      QSpinBox* lengthSpin;
   };
} // namespace GraphEdit

#endif // NOT GraphEditLengthH
