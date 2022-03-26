#include "GraphVisuWidget.h"

#include <QAction>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

#include "MainWidget.h"

GraphVisuWidget::GraphVisuWidget(MainWidget* mainWidget, QToolBar* toolBar)
   : AbstractWidget(mainWidget, toolBar, "visu")
{
   QLabel* label = new QLabel("Graph Visu", this);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(label);
}

