#include "RampDeviceRaspi.h"

#include <QTimer>

#include <Tools/Counter.h>

RampDevice::Raspi::Raspi(QObject* parent)
   : Midi::Tunnel::Client(parent, "192.168.1.180")
   , Abstract()
   , midiDevice(this)
   , stripA(midiDevice.create(0))
{
   QTimer* sendTimer = new QTimer(this);
   connect(sendTimer, &QTimer::timeout, this, &Raspi::slotAdvanceStep);
   sendTimer->start(1000);
}

void RampDevice::Raspi::pushToServer()
{
}

void RampDevice::Raspi::slotAdvanceStep()
{
   static const std::vector<std::vector<float>> voltages = {{5.0, 0, 0, 2.5}, {0, 0, 2.5, 5.0}, {2.5, 5.0, 0, 0}};
   static Counter counter(2);

   const uint8_t index = counter.valueAndNext();
   for (uint8_t port = 0; port < 3; port++)
   {
      stripA.setCV(port, voltages[index][port]);
   }
}
