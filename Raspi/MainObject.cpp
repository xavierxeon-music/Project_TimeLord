#include "MainObject.h"

#include <QCoreApplication>

#include <QTimer>

#include <ConsoleApplication.h>
#include <FileStorage.h>

const QString MainObject::storageFileName = "timelord.bin";
const uint16_t MainObject::callbackRate = 100;

MainObject::MainObject()
   : QObject(nullptr)
   , Remember::Root()
   , polyRamps(this)
   , server(this)
   , inputDevice("system:midi_capture_2")
   , outputDevice("system:midi_playback_2")
   , bridge(this, &server, &server, 10, 11)
#ifdef WAIT_FOR_FLAME_DEVICE
   , doepferQuad(&outputDevice)
   , quadStrips()
#endif
   , flameCC(&outputDevice)
   , tempo()
   , clockGate(&inputDevice, 10, Note::fromMidi(36))
   , resetGate(&inputDevice, 10, Note::fromMidi(37))
   , tickActive(false)
{
   qInfo() << "start";

   for (Midi::Channel channel = 0; channel < 4; channel++)
      quadStrips[channel] = doepferQuad.create(channel);

   inputDevice.initMidi();
   inputDevice.addPassThroughInterface(&outputDevice);

   outputDevice.initMidi();
   flameCC.init();

   server.addPassThroughInterface(&outputDevice);
   inputDevice.addPassThroughInterface(&server);

   QTimer* loopTimer = new QTimer(this);
   loopTimer->setTimerType(Qt::PreciseTimer);
   connect(loopTimer, &QTimer::timeout, this, &MainObject::loop);
   loopTimer->setInterval(callbackRate);
   loopTimer->start();

   FileStorage fileStorage(this);
   fileStorage.loadFromFile(storageFileName);

   bridge.onPulledFromRemote(this, &MainObject::reset);
}

MainObject::~MainObject()
{
   qInfo() << "end";
}

void MainObject::loop()
{
   if (resetGate.isOn())
   {
      tempo.clockReset();
      for (uint8_t index = 0; index < polyRamps.getSize(); index++)
         polyRamps[index].clockReset();

      tickActive = false;
   }
   else if (clockGate.isOn())
   {
      if (!tickActive)
      {
         tempo.clockTick();
         for (uint8_t index = 0; index < polyRamps.getSize(); index++)
            polyRamps[index].clockReset();
      }
      tickActive = true;
   }
   else
   {
      tempo.advance(callbackRate);
      tickActive = false;
   }

   for (uint8_t index = 0; index < polyRamps.getSize(); index++)
   {
      float voltage = 0.0;
      if (tempo.isRunningOrFirstTick())
      {
         const Tempo::Division division = polyRamps[index].getStepSize();
         const uint8_t value = polyRamps[index].getCurrentValue(tempo.getPercentage(division));
         voltage = value * 5.0 / 255.0;
      }
#ifdef WAIT_FOR_FLAME_DEVICE
      if (index < 8)
      {
         bool isPitch = (0 == index % 2);
         qDebug() << index << isPitch;
         if (isPitch)
         {
            const Midi::Channel channel = index / 2;
            quadStrips[channel].setCV12(voltage);
         }
         else
         {
            const Midi::Channel channel = (index - 1) / 2;
            quadStrips[channel].setCV3(voltage);
         }

         continue;
      }
#endif

      flameCC.setCV(index, voltage);
   }
}

void MainObject::reset()
{
   tempo.clockReset();
   for (uint8_t index = 0; index < polyRamps.getSize(); index++)
   {
      polyRamps[index].clockReset();
      flameCC.setCV(index, 0.0);
   }

#ifdef WAIT_FOR_FLAME_DEVICE
   for (Midi::Channel channel = 0; channel < 4; channel++)
   {
      quadStrips[channel].setCV12(0.0);
      quadStrips[channel].setCV3(0.0);
   }
#endif
   tickActive = false;
}

// main

int main(int argc, char** argv)
{
   QCoreApplication app(argc, argv);
   ConsoleApplication::catchSignals();

   MainObject mo;

   return app.exec();
}
