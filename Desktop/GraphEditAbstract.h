#ifndef GraphEditAbstractH
#define GraphEditAbstractH

#include "DataCore.h"
#include <QToolBar>

#include <QHBoxLayout>

class GraphWidget;

namespace GraphEdit
{
   class Abstract : public QToolBar, public DataCore
   {
      Q_OBJECT
   public:
      Abstract(GraphWidget* graphWidget, MainWidget* mainWidget);

   protected:
      void setPayload(QWidget* widget, const QString& text);
      virtual void execute(Graph* graph) = 0;

   private slots:
      void slotCancel();
      void slotExecute();

   private:
      GraphWidget* graphWidget;
   };
} // namespace GraphEdit

#endif // NOT GraphEditAbstractH
