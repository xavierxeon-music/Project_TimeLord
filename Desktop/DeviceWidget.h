#ifndef DeviceWidgetH
#define DeviceWidgetH

#include "AbstractWidget.h"

class DeviceModel;

class DeviceWidget : public AbstractWidget
{
   Q_OBJECT
public:
   DeviceWidget(MainWidget* mainWidget, QToolBar* toolBar, DeviceModel* deviceModel);

signals:
   void signalPortChanged(const Provider provider, const uint8_t& index);

private slots:
   void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

private:
   DeviceModel* deviceModel;
};

#endif // DeviceWidgetH
