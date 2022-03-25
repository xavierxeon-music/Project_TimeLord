#include "GraphWidget.h"

#include <QAction>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

#include "MainWidget.h"

GraphWidget::GraphWidget(MainWidget* mainWidget, QToolBar* toolBar)
   : AbstractWidget(mainWidget, toolBar, "graph")
{
   QLabel* label = new QLabel("Graph", this);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(label);
}

