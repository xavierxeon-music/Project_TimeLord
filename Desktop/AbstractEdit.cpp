#include "AbstractEdit.h"

#include <QIcon>
#include <QLabel>

#include "MainWidget.h"
#include "RampWidget.h"

Abstract::Edit::Edit(Ramp::Widget* polyRampWidget)
   : QToolBar(polyRampWidget)
   , Core::Interface()
   , polyRampWidget(polyRampWidget)
{
   setIconSize(QSize(24, 24));
}

void Abstract::Edit::setPayload(QWidget* widget, const QString& text)
{
   addAction(QIcon(":/Cancel.svg"), "Cancel", this, &Edit::slotCancel);
   addAction(QIcon(":/Execute.svg"), "Execute", this, &Edit::slotExecute);

   addWidget(new QLabel(text));

   widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
   addWidget(widget);
}

void Abstract::Edit::slotCancel()
{
   polyRampWidget->hideEditStack();
}

void Abstract::Edit::slotExecute()
{
   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
   {
      Core::Identifier identifier(rampIndex);

      PolyRamp* polyRamp = getPolyRamp(identifier);
      execute(polyRamp);
   }

   polyRampWidget->hideEditStack();
   callOnAllInstances(&Interface::rebuildModel, Core::Identifier());
}
