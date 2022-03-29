#include "EditAbstract.h"

#include <QLabel>

#include "MainWidget.h"
#include "PolyRampWidget.h"

Edit::Abstract::Abstract(PolyRampWidget* polyRampWidget, MainWidget* mainWidget)
   : QToolBar(polyRampWidget)
   , Data::Core(mainWidget)
   , polyRampWidget(polyRampWidget)
{
   setIconSize(QSize(24, 24));
}

void Edit::Abstract::setPayload(QWidget* widget, const QString& text)
{
   addAction(QIcon(":/Cancel.svg"), "Cancel", this, &Abstract::slotCancel);
   addAction(QIcon(":/Execute.svg"), "Execute", this, &Abstract::slotExecute);

   addWidget(new QLabel(text));

   widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
   addWidget(widget);
}

void Edit::Abstract::slotCancel()
{
   polyRampWidget->hideEditStack();
}

void Edit::Abstract::slotExecute()
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
