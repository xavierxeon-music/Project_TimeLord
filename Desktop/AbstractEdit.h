#ifndef EditAbstractH
#define EditAbstractH

#include "DataCore.h"
#include <QToolBar>

#include <QHBoxLayout>

namespace Ramp
{
   class Widget;
}

namespace Abstract
{
   class Edit : public QToolBar, public Data::Core
   {
      Q_OBJECT
   public:
      Edit(Ramp::Widget* polyRampWidget);

   protected:
      void setPayload(QWidget* widget, const QString& text);
      virtual void execute(PolyRamp* polyRamp) = 0;

   private slots:
      void slotCancel();
      void slotExecute();

   private:
      Ramp::Widget* polyRampWidget;
   };
} // namespace Edit

#endif // NOT EditAbstractH
