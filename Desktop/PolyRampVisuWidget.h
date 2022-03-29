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
      using Map = QMap<Graph*, List>;
   };

private slots:
   void slotUpdate();
   void slotZoomIn();
   void slotZoomOut();

private:
   QGraphicsView* graphicsView;
   Stage::Map stageMap;

   Model::Provider selectedProvider;
   uint8_t selectedGraphIndex;

   uint8_t zoomLevel;
};

#endif // GraphVisuWidgetH
