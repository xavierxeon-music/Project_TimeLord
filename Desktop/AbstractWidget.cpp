#include "AbstractWidget.h"

#include <QFrame>
#include <QLabel>

#include "MainWidget.h"

AbstractWidget::AbstractWidget(MainWidget* mainWidget, QToolBar* toolBar, const QString& toolBarSectionName)
   : QWidget(mainWidget)
   , DataCore(mainWidget)
{
   if (0 != toolBar->actions().count())
   {
      QFrame* divider = new QFrame(this);
      divider->setFrameShape(QFrame::VLine);
      divider->setFixedWidth(5);
      toolBar->addWidget(divider);

      QWidget* spacer1 = new QWidget(this);
      spacer1->setFixedWidth(10);
      toolBar->addWidget(spacer1);
   }
   toolBar->addWidget(new QLabel(toolBarSectionName));

   QWidget* spacer2 = new QWidget(this);
   spacer2->setFixedWidth(10);
   toolBar->addWidget(spacer2);
}
