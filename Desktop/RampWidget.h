#ifndef RampWidgetH
#define RampWidgetH

#include "AbstractWidget.h"

#include <QStackedWidget>

#include "EditDivision.h"
#include "EditLength.h"
#include "EditLoop.h"

namespace Ramp
{
   class Model;

   class Widget : public Abstract::Widget
   {
      Q_OBJECT
   public:
      Widget(MainWidget* mainWidget);

   public:
      void hideEditStack();

   private slots:
      void slotTrimCurrentGraph();
      void slotSetLengthAllGraphs();
      void slotSetDivisionAllGraphs();
      void slotSetLoopAllGraphs();

   private:
      void selectionChanged(Core::Identifier identifier) override;

   private:
      Model* rampModel;
      QStackedWidget* editStack;
      Edit::Length* lengthEdit;
      Edit::Division* divisionEdit;
      Edit::Loop* loopEdit;

      Core::Identifier identifier;
   };
} // namespace Ramp

#endif // RampWidgetH
