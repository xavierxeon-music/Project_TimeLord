#include "AbstractEdit.h"

#include <QIcon>
#include <QLabel>

#include "MainWidget.h"
#include "RampWidget.h"

Abstract::Edit::Edit(Ramp::Widget* polyRampWidget, MainWidget* mainWidget)
   : QToolBar(polyRampWidget)
   , Data::Core(mainWidget)
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
   const PoviderNameMap& nameMap = getProviderNames();
   for (PoviderNameMap::const_iterator it = nameMap.constBegin(); it != nameMap.constEnd(); it++)
   {
      const Data::Provider provider = it.key();

      for (uint8_t graphIndex = 0; graphIndex < 16; graphIndex++)
      {
         PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
         execute(polyRamp);
      }
   }

   polyRampWidget->hideEditStack();
   mainWidget->forceRebuildModels();
}
