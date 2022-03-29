#include "GraphEditAbstract.h"

#include <QLabel>

#include "GraphWidget.h"
#include "MainWidget.h"

GraphEdit::Abstract::Abstract(GraphWidget* graphWidget, MainWidget* mainWidget)
   : QToolBar(graphWidget)
   , DataCore(mainWidget)
   , graphWidget(graphWidget)
{
   setIconSize(QSize(24, 24));
}

void GraphEdit::Abstract::setPayload(QWidget* widget, const QString& text)
{
   addAction(QIcon(":/Cancel.svg"), "Cancel", this, &Abstract::slotCancel);
   addAction(QIcon(":/Execute.svg"), "Execute", this, &Abstract::slotExecute);

   addWidget(new QLabel(text));

   widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
   addWidget(widget);
}

void GraphEdit::Abstract::slotCancel()
{
   graphWidget->hideEditStack();
}

void GraphEdit::Abstract::slotExecute()
{
   const PoviderNameMap& nameMap = getProviderNames();
   for (PoviderNameMap::const_iterator it = nameMap.constBegin(); it != nameMap.constEnd(); it++)
   {
      const Model::Provider provider = it.key();

      for (uint8_t graphIndex = 0; graphIndex < 16; graphIndex++)
      {
         Graph* graph = getGraph(provider, graphIndex);
         execute(graph);
      }
   }

   graphWidget->hideEditStack();
   mainWidget->forceRebuildModels();
}
