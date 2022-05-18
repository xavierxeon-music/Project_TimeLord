#include "AbstractEdit.h"

#include <QIcon>
#include <QLabel>

#include "MainWidget.h"
#include "RampWidget.h"

Abstract::Edit::Edit(Ramp::Widget* rampWidget)
   : QToolBar(rampWidget)
   , Core::Interface()
   , rampWidget(rampWidget)
   , selectionIdentifier()
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
   rampWidget->hideEditStack();
}

void Abstract::Edit::slotExecute()
{
   Core::Identifier identifier = selectionIdentifier;
   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
   {
      identifier.rampIndex = rampIndex;
      PolyRamp* polyRamp = getPolyRamp(identifier);
      if (!polyRamp)
         continue;

      execute(polyRamp);
   }

   rampWidget->hideEditStack();
   callOnAllInstances(&Interface::rebuildModel, Core::Identifier());
}

void Abstract::Edit::selectionChanged(Core::Identifier identifier)
{
   selectionIdentifier = identifier;
}
