#ifndef GraphWidgetH
#define GraphWidgetH

#include "DataCore.h"
#include <QWidget>

class GraphWidget : public QWidget, public DataCore
{
   Q_OBJECT
public:
   GraphWidget(MainWidget* mainWidget);
};

#endif // GraphWidgetH
