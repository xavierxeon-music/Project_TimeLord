#ifndef PointWidgetH
#define PointWidgetH

#include "AbstractWidget.h"

class PointModel;

class PointWidget : public AbstractWidget
{
   Q_OBJECT
public:
   PointWidget(MainWidget* mainWidget, QToolBar* toolBar, PointModel* pointModel);

signals:
   void signalPointSelected(const uint8_t& index);

private slots:
   void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

private:
   PointModel* pointModel;
};

#endif // PointWidgetH
