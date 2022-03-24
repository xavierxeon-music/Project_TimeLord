#ifndef GraphDeviceH
#define GraphDeviceH

#include <Remember.h>
#include <QObject>

#include <AudioDevice.h>

#include "MainWidget.h"

class GraphDevice : public QObject, public Remember::Root
{
   Q_OBJECT
public:
   GraphDevice(QObject* parent);

private:
   friend class DataCore;

private:
   MainWidget::GraphList_ graphs;
   AudioDevice::Driver audioDriver;
};

#endif // NOT GraphDeviceH
