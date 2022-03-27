#include "GraphEditLoop.h"

GraphEdit::Loop::Loop(GraphWidget* graphWidget, MainWidget* mainWidget)
   : Abstract(graphWidget, mainWidget)
   , loopCheck(nullptr)
{
   loopCheck = new QCheckBox(this);
   setPayload(loopCheck, "Loop");
}

void GraphEdit::Loop::execute(Graph* graph)
{
   const bool loop = loopCheck->isChecked();
   graph->setLooping(loop);
}
