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
   void audioLoop(const float& audioCallbackRate);

private:
   MainWidget::GraphList_ graphs;
   AudioDevice::Driver audioDriver;
   AudioDevice::InputsTempo tempo;
   AudioDevice::OutputCV outputs[16];
};

#endif // NOT GraphAudioDeviceH
