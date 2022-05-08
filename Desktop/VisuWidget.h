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
} // namespace Ramp

#endif // PolyRampVisuH
