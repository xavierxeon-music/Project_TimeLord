#ifndef MainWidgetH
#define MainWidgetH

#include <CompanionApp.h>
#include <TimeLord.h>
#include <QSplitter>

#include "GraphDevice.h"
#include "GraphWidget.h"
#include "PointModel.h"
#include "PointWidget.h"
#include "PortModel.h"
#include "PortWidget.h"

class MainWidget : public QSplitter, public CompanionApp, public TimeLord
{
   Q_OBJECT
public:
   MainWidget();

public slots:
   void slotLoadFromFile();
   void slotStartLoadFromdDaisy();
   void slotSaveToFile();
   void slotSaveToDaisy();

private:
   void loadedFromDaisy() override;
   void updateUI();
   void closeEvent(QCloseEvent* ce) override;

private:
   PortWidget* portWidget;
   PortModel* portModel;

   PointWidget* pointWidget;
   PointModel* pointModel;

   GraphWidget* graphWidget;
   GraphDevice* graphDevice;
};

#endif // MainWidgetH
