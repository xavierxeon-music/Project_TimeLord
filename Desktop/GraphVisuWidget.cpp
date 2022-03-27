#include "GraphVisuWidget.h"

#include <QAction>
#include <QGraphicsView>
#include <QToolBar>

#include "MainWidget.h"

GraphVisuWidget::GraphVisuWidget(MainWidget* mainWidget)
   : AbstractWidget(mainWidget)
{
   toolBar->addAction(QIcon(":/ZoomIn.svg"), "Zoom In");
   toolBar->addAction(QIcon(":/ZoomOut.svg"), "Zoom Out");

   QGraphicsView* graphicsView = new QGraphicsView(this);
   addPayload(graphicsView);
}

