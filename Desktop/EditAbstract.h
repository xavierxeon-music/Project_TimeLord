#ifndef EditAbstractH
#define EditAbstractH

#include "DataCore.h"
#include <QToolBar>

#include <QHBoxLayout>

class PolyRampWidget;

namespace Edit
{
   class Abstract : public QToolBar, public DataCore
   {
      Q_OBJECT
   public:
      Abstract(PolyRampWidget* polyRampWidget, MainWidget* mainWidget);

   protected:
      void setPayload(QWidget* widget, const QString& text);
      virtual void execute(PolyRamp* polyRamp) = 0;

   private slots:
      void slotCancel();
      void slotExecute();

   private:
      PolyRampWidget* polyRampWidget;
   };
} // namespace Edit

#endif // NOT EditAbstractH
