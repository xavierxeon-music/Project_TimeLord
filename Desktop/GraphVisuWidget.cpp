#include "GraphVisuWidget.h"

#include <QAction>
#include <QGraphicsView>
#include <QToolBar>

#include "MainWidget.h"

GraphVisuWidget::GraphVisuWidget(MainWidget* mainWidget)
   : AbstractWidget(mainWidget)
{
   toolBar->addAction(QIcon(":/Add.svg"), "Hello");

   QGraphicsView* graphicsView = new QGraphicsView(this);
   addPayload(graphicsView);
}

