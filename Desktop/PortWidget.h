#ifndef PortWidgetH
#define PortWidgetH

#include "DataCore.h"
#include <QWidget>

class PortModel;

class PortWidget : public QWidget, public DataCore
{
   Q_OBJECT
public:
   PortWidget(MainWidget* mainWidget, PortModel* portModel);

signals:
   void signalPortChanged(const Provider provider, const uint8_t& index);

private slots:
   void slotCurrentSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

private:
   PortModel* portModel;
};

#endif // PortWidgetH
