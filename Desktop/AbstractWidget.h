#ifndef AbstractWidgetH
#define AbstractWidgetH

#include "DataCore.h"
#include <QWidget>

#include <QToolBar>

class MainWidget;

class AbstractWidget : public QWidget, public DataCore
{
   Q_OBJECT
public:
   AbstractWidget(MainWidget* mainWidget, QToolBar* toolBar, const QString& toolBarSectionName);
};

#endif // AbstractWidgetH
