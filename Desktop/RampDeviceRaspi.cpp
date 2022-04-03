#include "RampDeviceRaspi.h"

#include <QTimer>

#include <Tools/Counter.h>

#include <Sound/StandardTable.h>
#include <Sound/TableOscilator.h>
#include <Tools/Range.h>

RampDevice::Raspi::Raspi(QObject* parent)
   : Midi::Tunnel::Client(parent, "192.168.1.180")
   , Abstract()
   , midiDevice(this)
   , stripA(midiDevice.create(0))
{
   const float sampleRate = 10;

   one.init(Standard::getTable(Standard::Waveform::Sine), sampleRate);
   one.setCycleDuration(1.0);
   one.setAmplitude(1.0);

   two.init(Standard::getTable(Standard::Waveform::Sine), sampleRate);
   two.setCycleDuration(2.0);
   two.setAmplitude(1.0);

   QTimer* sendTimer = new QTimer(this);
   connect(sendTimer, &QTimer::timeout, this, &Raspi::slotAdvanceStep);
   sendTimer->start(1000.0 / sampleRate);
}

void RampDevice::Raspi::pushToServer()
{
}

void RampDevice::Raspi::slotAdvanceStep()
{
   static const Range::Mapper mapper(-1.0, 1.0, 0.0, 5.0);
   const float voltage1 = mapper(one.createSound());
   stripA.setCV12(voltage1);

   const float voltage2 = mapper(two.createSound());
   stripA.setCV3(voltage2);
}
