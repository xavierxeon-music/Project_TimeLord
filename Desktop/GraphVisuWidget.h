#ifndef GraphVisuWidgetH
#define GraphVisuWidgetH

#include "AbstractWidget.h"

#include <QGraphicsView>

class GraphVisuWidget : public AbstractWidget
{
   Q_OBJECT
public:
   GraphVisuWidget(MainWidget* mainWidget);

public slots:
   void slotGraphSelected(const Model::Provider& newProvider, const uint8_t& newGraphIndex);

private:
   struct Stage
   {
      QGraphicsLineItem* lineItem;

      using List = QList<Stage>;
   };

private slots:
   void slotUpdate();

private:
   QGraphicsView* graphicsView;
   Stage::List stageList;
   Graph* graph;
};

#endif // GraphVisuWidgetH
