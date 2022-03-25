#ifndef GraphAudioDeviceH
#define GraphAudioDeviceH

#include <Remember.h>
#include <QObject>

#include <AudioDevice.h>

#include "MainWidget.h"

// not named AudioDevice to avoid name clashes!
class GraphAudioDevice : public QObject, public Remember::Root
{
   Q_OBJECT
public:
   GraphAudioDevice(QObject* parent);

private:
   friend class DataCore;

private:
   MainWidget::GraphList_ graphs;
   AudioDevice::Driver audioDriver;
};

#endif // NOT GraphAudioDeviceH
