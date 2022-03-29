#ifndef EditLoopH
#define EditLoopH

#include "EditAbstract.h"

#include <QCheckBox>

namespace Edit
{
   class Loop : public Abstract
   {
   public:
      Loop(PolyRampWidget* polyRampWidget, MainWidget* mainWidget);

   private:
      void execute(Graph* graph) override;

   private:
      QCheckBox* loopCheck;
   };
} // namespace Edit

#endif // NOT EditLoopH
