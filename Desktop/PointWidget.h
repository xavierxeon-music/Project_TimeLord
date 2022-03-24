#ifndef PointWidgetH
#define PointWidgetH

#include "DataCore.h"
#include <QWidget>

class PointModel;

class PointWidget : public QWidget, public DataCore
{
   Q_OBJECT
public:
   PointWidget(MainWidget* mainWidget, PointModel* pointModel);

signals:
   void signalPointSelected(const uint8_t& index);

private slots:
   void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);
};

#endif // PointWidgetH
