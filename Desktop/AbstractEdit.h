#ifndef EditAbstractH
#define EditAbstractH

#include "Core.h"
#include <QToolBar>

#include <QHBoxLayout>

namespace Ramp
{
   class Widget;
}

namespace Abstract
{
   class Edit : public QToolBar, public Core::Interface
   {
      Q_OBJECT
   public:
      Edit(Ramp::Widget* rampWidget);

   protected:
      void setPayload(QWidget* widget, const QString& text);
      virtual void execute(PolyRamp* polyRamp) = 0;
      void selectionChanged(Core::Identifier identifier) override;

   private slots:
      void slotCancel();
      void slotExecute();

   private:
      Ramp::Widget* rampWidget;
      Core::Identifier selectionIdentifier;
   };
} // namespace Abstract

#endif // NOT EditAbstractH
