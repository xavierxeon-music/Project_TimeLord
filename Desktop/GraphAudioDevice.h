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

signals:
   void signalStatusUpdate(const Tempo::RunState& runState, const uint8_t& beatsPerMinute);

private:
   friend class DataCore;

private slots:
   void slotStatusUpdate();

private:
   void audioLoop(const float& audioCallbackRate);
   void clockTick();
   void clockReset();

private:
   QStatusBar* statusBar;

   MainWidget::GraphList_ graphs;
   AudioDevice::Driver audioDriver;
   AudioDevice::InputsTempo tempo;
   AudioDevice::OutputCV outputs[16];
};

#endif // NOT GraphAudioDeviceH
