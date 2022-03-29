#ifndef AudioDeviceGraphH
#define AudioDeviceGraphH

#include <Remember.h>
#include <QObject>

#include <AudioDevice.h>

#include "MainWidget.h"

// not named AudioDevice to avoid name clashes!
class AudioDeviceGraph : public QObject, public Remember::Root
{
   Q_OBJECT
public:
   AudioDeviceGraph(QObject* parent);

public:
   void clockReset();
   const Tempo* getTempo() const;

private:
   friend class DataCore;

private: // things to remeber
   using PolyRampList_ = Remember::RefArray<PolyRamp, 16>;

private:
   void audioLoop(const float& audioCallbackRate);
   void clockTick();

private:
   QStatusBar* statusBar;

   PolyRampList_ polyRamps;
   AudioDevice::Driver audioDriver;
   AudioDevice::InputsTempo tempo;
   AudioDevice::OutputCV outputs[16];
};

#endif // NOT AudioDeviceGraphH
