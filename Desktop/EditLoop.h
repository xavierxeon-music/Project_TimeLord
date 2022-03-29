#ifndef GraphEditLoopH
#define GraphEditLoopH

#include "GraphEditAbstract.h"

#include <QCheckBox>

namespace GraphEdit
{
   class Loop : public Abstract
   {
   public:
      Loop(GraphWidget* graphWidget, MainWidget* mainWidget);

   private:
      void execute(Graph* graph) override;

   private:
      QCheckBox* loopCheck;
   };
} // namespace GraphEdit

#endif // NOT GraphEditLoopH
