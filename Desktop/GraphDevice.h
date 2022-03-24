#ifndef GraphDeviceH
#define GraphDeviceH

#include <QObject>

#include <AudioDevice.h>

class GraphDevice : public QObject
{
   Q_OBJECT
public:
   GraphDevice(QObject* parent);

private:
   AudioDevice::Driver audioDriver;
};

#endif // NOT GraphDeviceH
