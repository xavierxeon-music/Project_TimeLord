#include "RampVisu.h"

#include <QAction>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QSpinBox>
#include <QTimer>

#include "MainWidget.h"
#include "RampDeviceVCV.h"

Ramp::Visu::Visu(MainWidget* mainWidget)
   : Abstract::Widget(mainWidget)
   , graphicsView(nullptr)
   , stageMap()
   , identifier()
   , zoomLevel(10)
{
   setFixedHeight(200);

   const MainWidget::FileActions& fileActions = mainWidget->getFileActions();

   toolBar->addAction(fileActions.loadFromFile);
   toolBar->addAction(fileActions.saveToFile);
   toolBar->addAction(fileActions.saveNewFile);

   toolBar->addSeparator();

   const RampDevice::VCV::ServerActions& serverActions = mainWidget->getServerActions();
   toolBar->addAction(serverActions.connectToServer);
   toolBar->addAction(serverActions.bankUp);
   toolBar->addAction(serverActions.pushToServer);

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

void Ramp::Visu::slotUpdate()
{
   static const QPen blackPen(QColor(0, 0, 0), 2);
   static const QPen grayPen(QColor(200, 200, 200));

   PolyRamp* selectedPolyRamp = getPolyRamp(identifier);

   auto drawGraph = [&](PolyRamp* polyRamp)
   {
      if (!polyRamp || 0 == polyRamp->getStageCount())
         return;

      Stage::List& stageList = stageMap[polyRamp];
      while (stageList.size() < polyRamp->getStageCount()) // add lines
      {
         Stage stage;
         stage.lineItem = new QGraphicsLineItem();
         graphicsView->scene()->addItem(stage.lineItem);

         stageList.append(stage);
      }

      while (stageList.size() > polyRamp->getStageCount()) // remove lines
      {
         Stage stage = stageList.takeLast();
         delete stage.lineItem;
      }

      const uint16_t offsetY = 10;
      const uint32_t offsetX = 5;

      uint32_t startX = offsetX;
      for (uint8_t startIndex = 0; startIndex < polyRamp->getStageCount(); startIndex++)
      {
         uint32_t stageLength = polyRamp->getStageLength(startIndex);
         uint8_t endIndex = (startIndex + 1 < polyRamp->getStageCount()) ? startIndex + 1 : 0;

         uint32_t startY = 128 - (0.5 * polyRamp->getStageStartHeight(startIndex));
         uint32_t endY = 128 - (0.5 * polyRamp->getStageStartHeight(endIndex));

         uint32_t endX = (endIndex == 0) ? offsetX + polyRamp->getLength() : startX + stageLength;

         QGraphicsLineItem* lineItem = stageList[startIndex].lineItem;
         lineItem->setLine(zoomLevel * startX, offsetY + startY, zoomLevel * endX, offsetY + endY);
         lineItem->setPen((polyRamp == selectedPolyRamp) ? blackPen : grayPen);
         lineItem->setZValue((polyRamp == selectedPolyRamp) ? 1.0 : 0.0);

         startX = endX;
      }
   };

   for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
   {
      Data::Identifier drawIdentifier(rampIndex);
      PolyRamp* polyRamp = getPolyRamp(drawIdentifier);
      if (selectedPolyRamp == polyRamp)
         continue;

      drawGraph(polyRamp);
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

void Ramp::Visu::polyRampSelected(Data::Identifier newIdentifier)
{
   identifier = newIdentifier;

   slotUpdate();
}
