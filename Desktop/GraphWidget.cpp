#include "GraphWidget.h"

#include <QAction>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

#include "MainWidget.h"

GraphWidget::GraphWidget(MainWidget* mainWidget)
   : QWidget(mainWidget)
   , DataCore(mainWidget)
{
   QToolBar* toolBar = new QToolBar(this);
   toolBar->setIconSize(QSize(24, 24));

   QLabel* label = new QLabel("Graph", this);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(toolBar);
   masterLayout->addWidget(label);
}

