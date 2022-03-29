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
      void slotGraphSelected(const Data::Provider& newProvider, const uint8_t& newGraphIndex);

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

      Data::Provider selectedProvider;
      uint8_t selectedGraphIndex;

      uint8_t zoomLevel;
   };
} // namespace Ramp

#endif // PolyRampVisuH
