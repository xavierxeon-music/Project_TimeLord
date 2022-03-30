#ifndef RampVisuH
#define RampVisuH

#include "AbstractWidget.h"

#include <QGraphicsView>

namespace Ramp
{
   class Visu : public Abstract::Widget
   {
      Q_OBJECT
   public:
      Visu(MainWidget* mainWidget);

   public slots:
      void slotGraphSelected(const Data::Identifier& newIdentifier);

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
      QGraphicsView* graphicsView;
      Stage::Map stageMap;

      Data::Identifier identifier;
      uint8_t zoomLevel;
   };
} // namespace Ramp

#endif // PolyRampVisuH
