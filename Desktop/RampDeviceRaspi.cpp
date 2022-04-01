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
   using VoltageList = std::vector<float>;
   static const std::vector<VoltageList> voltageMap = {{4.9, 0.0, 2.5}, {0.0, 2.5, 4.9}, {2.5, 4.9, 0.0}};
   static Counter counter(3);

   const uint8_t index = counter.valueAndNext();
   const VoltageList& voltageList = voltageMap.at(index);
   for (uint8_t port = 0; port < 3; port++)
   {
      const float& voltage = voltageList.at(port);
      stripA.setCV(port, voltage);
   }
}
