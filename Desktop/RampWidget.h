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
      Widget(MainWidget* mainWidget, Model* polyRampModel);

   signals:
      void signalGraphSelected(const Data::Provider provider, const uint8_t& index);

   public:
      void hideEditStack();

   private slots:
      void slotTrimCurrentGraph();
      void slotSetLengthAllGraphs();
      void slotSetDivisionAllGraphs();
      void slotSetLoopAllGraphs();
      void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

   private:
      Model* polyRampModel;
      QStackedWidget* editStack;
      Edit::Length* lengthEdit;
      Edit::Division* divisionEdit;
      Edit::Loop* loopEdit;
      Data::Provider selectedProvider;
      int selectedGraphIndex;
   };
} // namespace Ramp

#endif // RampWidgetH
