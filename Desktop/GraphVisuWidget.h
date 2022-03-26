#ifndef GraphVisuWidgetH
#define GraphVisuWidgetH

#include "AbstractWidget.h"

class GraphVisuWidget : public AbstractWidget
{
   Q_OBJECT
public:
   GraphVisuWidget(MainWidget* mainWidget, QToolBar* toolBar);
};

#endif // GraphVisuWidgetH
