#include "RampVisu.h"

#include <QAction>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QTimer>

#include "MainWidget.h"

Ramp::Visu::Visu(MainWidget* mainWidget)
   : Abstract::Widget(mainWidget)
   , graphicsView(nullptr)
   , stageMap()
   , identifier()
   , zoomLevel(10)
{
   setFixedHeight(200);

   toolBar->addAction(QIcon(":/LoadFromFile.svg"), "Load From File", mainWidget, &MainWidget::slotLoadFromFile);
   QAction* saveFileAction = toolBar->addAction(QIcon(":/SaveToFile.svg"), "Save To File", mainWidget, &MainWidget::slotSaveToFile);
   saveFileAction->setShortcut(QKeySequence(QKeySequence::Save));
   toolBar->addAction(QIcon(":/SaveNewFile.svg"), "Save To New File", mainWidget, &MainWidget::slotSaveNewFile);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/SaveToDaisy.svg"), "Save To RasPi", mainWidget, &MainWidget::slotSaveToRaspi);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/ZoomIn.svg"), "Zoom In", this, &Visu::slotZoomIn);
   toolBar->addAction(QIcon(":/ZoomOut.svg"), "Zoom Out", this, &Visu::slotZoomOut);

   QGraphicsScene* scene = new QGraphicsScene(this);
   scene->setSceneRect(0, 0, 150, 150);
   graphicsView = new QGraphicsView(scene, this);
   addPayload(graphicsView);

   QTimer* updateTimer = new QTimer(this);
   connect(updateTimer, &QTimer::timeout, this, &Visu::slotUpdate);
   updateTimer->start(500);

   QPen whitePen(QColor(255, 255, 255));
   graphicsView->scene()->addLine(0, 0, 0, 150, whitePen); // to force height even withou graph data
}

void Ramp::Visu::slotGraphSelected(const Data::Identifier& newIdentifier)
{
   identifier = newIdentifier;

   slotUpdate();
}

void Ramp::Visu::slotUpdate()
{
   static const QPen blackPen(QColor(0, 0, 0), 2);
   static const QPen grayPen(QColor(200, 200, 200));

   PolyRamp* selectedPolyRamp = getPolyRamp(identifier);

   auto drawGraph = [&](PolyRamp* polyRamp)
   {
      if (!polyRamp || 0 == polyRamp->stageCount())
         return;

      Stage::List& stageList = stageMap[polyRamp];
      while (stageList.size() < polyRamp->stageCount()) // add lines
      {
         Stage stage;
         stage.lineItem = new QGraphicsLineItem();
         graphicsView->scene()->addItem(stage.lineItem);

         stageList.append(stage);
      }

      while (stageList.size() > polyRamp->stageCount()) // remove lines
      {
         Stage stage = stageList.takeLast();
         delete stage.lineItem;
      }

      uint16_t offsetY = 10;
      uint32_t startX = 5;

      for (uint8_t startIndex = 0; startIndex < polyRamp->stageCount(); startIndex++)
      {
         uint32_t stageLength = polyRamp->getStageLength(startIndex);
         uint8_t endIndex = (startIndex + 1 < polyRamp->stageCount()) ? startIndex + 1 : 0;

         uint32_t startY = 128 - (0.5 * polyRamp->getStageStartHeight(startIndex));
         uint32_t endY = 128 - (0.5 * polyRamp->getStageStartHeight(endIndex));

         uint32_t endX = (endIndex == 0) ? polyRamp->getLength() : startX + stageLength;

         QGraphicsLineItem* lineItem = stageList[startIndex].lineItem;
         lineItem->setLine(zoomLevel * startX, offsetY + startY, zoomLevel * endX, offsetY + endY);
         lineItem->setPen((polyRamp == selectedPolyRamp) ? blackPen : grayPen);
         lineItem->setZValue((polyRamp == selectedPolyRamp) ? 1.0 : 0.0);

         startX = endX;
      }
   };

   const DeviceNameMap& nameMap = getDeviceNameMap();
   for (DeviceNameMap::const_iterator it = nameMap.constBegin(); it != nameMap.constEnd(); it++)
   {
      Data::Identifier drawIdentifier(it.key());
      for (uint8_t rampIndex = 0; rampIndex < 16; rampIndex++)
      {
         drawIdentifier.rampIndex = rampIndex;
         PolyRamp* polyRamp = getPolyRamp(drawIdentifier);
         drawGraph(polyRamp);
      }
   }

   drawGraph(selectedPolyRamp);

   QRectF contentRect = graphicsView->scene()->itemsBoundingRect();
   contentRect.setHeight(150);
   graphicsView->scene()->setSceneRect(contentRect);
}

void Ramp::Visu::slotZoomIn()
{
   if (255 == zoomLevel)
      return;

   zoomLevel++;
   slotUpdate();
}

void Ramp::Visu::slotZoomOut()
{
   if (1 == zoomLevel)
      return;

   zoomLevel--;
   slotUpdate();
}
