#ifndef AbstractWidgetH
#define AbstractWidgetH

#include "DataCore.h"
#include <QWidget>

#include <QToolBar>
#include <QVBoxLayout>

class MainWidget;

class AbstractWidget : public QWidget, public Data::Core
{
   Q_OBJECT
public:
   AbstractWidget(MainWidget* mainWidget);

protected:
   void addPayload(QWidget* widget);

protected:
   QToolBar* toolBar;

private:
   QVBoxLayout* masterLayout;
};

#endif // AbstractWidgetH
