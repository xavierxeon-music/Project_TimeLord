#include "PolyLineModel.h"

#include "MainWidget.h"

PolyLine::Model::Model(MainWidget* mainWidget)
   : QStandardItemModel(mainWidget)
   , Data::Core(mainWidget)
{
   setHorizontalHeaderLabels({"index", "position", "length", "height"});
}

void PolyLine::Model::rebuild(const Data::Provider provider, const uint8_t& graphIndex, bool lengthChanged)
{
   clear();
   setHorizontalHeaderLabels({"index", "position", "length", "height"});

   PolyRamp* polyRamp = getPolyRamp(provider, graphIndex);
   if (!polyRamp)
      return;

   QStandardItem* parentItem = new QStandardItem(polyRamp->stageCount());
   parentItem->setEditable(false);

   for (uint8_t stageIndex = 0; stageIndex < polyRamp->stageCount(); stageIndex++)
   {
   }
}
