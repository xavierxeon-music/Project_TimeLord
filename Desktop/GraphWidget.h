#ifndef GraphWidgetH
#define GraphWidgetH

#include "AbstractWidget.h"

class GraphWidget : public AbstractWidget
{
   Q_OBJECT
public:
   GraphWidget(MainWidget* mainWidget, QToolBar* toolBar);
};

#endif // GraphWidgetH
