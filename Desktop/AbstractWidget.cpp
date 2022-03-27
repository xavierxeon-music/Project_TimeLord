#include "AbstractWidget.h"

#include <QFrame>
#include <QLabel>

#include "MainWidget.h"

AbstractWidget::AbstractWidget(MainWidget* mainWidget)
   : QWidget(mainWidget)
   , DataCore(mainWidget)
   , toolBar(nullptr)
   , masterLayout(nullptr)
{
   toolBar = new QToolBar(this);
   toolBar->setIconSize(QSize(24, 24));

   masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(toolBar);
}

void AbstractWidget::addPayload(QWidget* widget)
{
   masterLayout->addWidget(widget);
}
