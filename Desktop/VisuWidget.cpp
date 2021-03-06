#include "VisuWidget.h"

#include <QAction>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QSpinBox>
#include <QTimer>

#include "MainWidget.h"
#include "Target.h"

Visu::Widget::Widget(MainWidget* mainWidget)
   : Abstract::Widget(mainWidget)
   , graphicsView(nullptr)
   , stageMap()
   , identifier()
   , zoomLevel(10)
{
   setFixedHeight(200);

   const MainWidget::FileActions& fileActions = mainWidget->getFileActions();

   toolBar->addAction(fileActions.newFile);
   toolBar->addAction(fileActions.loadFromFile);
   toolBar->addAction(fileActions.saveToFile);
   toolBar->addAction(fileActions.saveNewFile);

   toolBar->addSeparator();

   const Target::ServerActions& serverActions = mainWidget->getServerActions();
   toolBar->addAction(serverActions.connectToServer);
   toolBar->addAction(serverActions.pushToServer);

   QAction* stateFromServer = new QAction(QIcon(":/StateFromMajordomo.svg"), "State From Rack", this);
   connect(stateFromServer, &QAction::triggered, this, &Visu::Widget::signalCaptureStates);
   toolBar->addAction(stateFromServer);

   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/ZoomIn.svg"), "Zoom In", this, &Visu::Widget::slotZoomIn);
   toolBar->addAction(QIcon(":/ZoomOut.svg"), "Zoom Out", this, &Visu::Widget::slotZoomOut);

   QGraphicsScene* scene = new QGraphicsScene(this);
   scene->setSceneRect(0, 0, 150, 150);
   graphicsView = new QGraphicsView(scene, this);
   addWidget(graphicsView);

   QTimer* updateTimer = new QTimer(this);
   connect(updateTimer, &QTimer::timeout, this, &Visu::Widget::slotUpdate);
   updateTimer->start(500);

   QPen whitePen(QColor(255, 255, 255));
   graphicsView->scene()->addLine(0, 0, 0, 150, whitePen); // to force height even withou graph data
}

uint8_t Visu::Widget::getZoom() const
{
   return zoomLevel;
}

void Visu::Widget::setZoom(const uint8_t level)
{
   zoomLevel = level;
   slotUpdate();
}

void Visu::Widget::slotUpdate()
{
   PolyRamp* selectedPolyRamp = getPolyRamp(identifier);

   for (uint8_t bankIndex = 0; bankIndex < getBankCount(); bankIndex++)
   {
      for (uint8_t rampIndex = 0; rampIndex < 8; rampIndex++)
      {
         Core::Identifier drawIdentifier(bankIndex, rampIndex);
         PolyRamp* polyRamp = getPolyRamp(drawIdentifier);
         if (selectedPolyRamp == polyRamp)
            continue;

         drawGraph(polyRamp, false);
      }
   }

   drawGraph(selectedPolyRamp, true);

   QRectF contentRect = graphicsView->scene()->itemsBoundingRect();
   contentRect.setHeight(150);
   graphicsView->scene()->setSceneRect(contentRect);
}

void Visu::Widget::drawGraph(PolyRamp* polyRamp, bool bold)
{
   static const QPen blackPen(QColor(0, 0, 0), 2);
   static const QPen grayPen(QColor(200, 200, 200));

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
   for (uint8_t index = 0; index < polyRamp->getStageCount(); index++)
   {
      uint32_t stageLength = polyRamp->getStageLength(index);

      uint32_t startY = 128 - (0.5 * polyRamp->getStageStartHeight(index));
      uint32_t endY = 128 - (0.5 * polyRamp->getStageEndHeight(index));

      uint32_t endX = (index + 1 == polyRamp->getStageCount()) ? offsetX + polyRamp->getLength() : startX + stageLength;

      QGraphicsLineItem* lineItem = stageList[index].lineItem;
      lineItem->setLine(zoomLevel * startX, offsetY + startY, zoomLevel * endX, offsetY + endY);
      lineItem->setPen(bold ? blackPen : grayPen);
      lineItem->setZValue(bold ? 1.0 : 0.0);

      startX = endX;
   }
}

void Visu::Widget::slotZoomIn()
{
   if (255 == zoomLevel)
      return;

   zoomLevel++;
   slotUpdate();

   setModified();
}

void Visu::Widget::slotZoomOut()
{
   if (1 == zoomLevel)
      return;

   zoomLevel--;
   slotUpdate();

   setModified();
}

void Visu::Widget::selectionChanged(Core::Identifier newIdentifier)
{
   identifier = newIdentifier;

   slotUpdate();
}
