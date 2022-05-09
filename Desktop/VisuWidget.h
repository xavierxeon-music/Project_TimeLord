#ifndef VisuWidgetH
#define VisuWidgetH

#include "AbstractWidget.h"

#include <QGraphicsView>

namespace Visu
{
   class Widget : public Abstract::Widget
   {
      Q_OBJECT
   public:
      Widget(MainWidget* mainWidget);

   private:
      struct Stage
      {
         QGraphicsLineItem* lineItem;

         using List = QList<Stage>;
         using Map = QMap<PolyRamp*, List>;
      };

   private slots:
      void slotUpdate();
      void slotZoomIn();
      void slotZoomOut();

   private:
      void polyRampSelected(Data::Identifier newIdentifier) override;

   private:
      QGraphicsView* graphicsView;
      Stage::Map stageMap;

      Data::Identifier identifier;
      uint8_t zoomLevel;
   };
} // namespace Visu

#endif // VisuWidgetH
