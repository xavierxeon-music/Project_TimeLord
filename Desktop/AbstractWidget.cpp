#include "AbstractWidget.h"

#include <QFrame>
#include <QLabel>

#include "MainWidget.h"

Abstract::Widget::Widget(MainWidget* mainWidget)
   : QWidget(mainWidget)
   , Data::Core()
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

void Abstract::Widget::addPayload(QWidget* widget)
{
   masterLayout->addWidget(widget);
}
