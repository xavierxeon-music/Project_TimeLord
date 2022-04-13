#include "MainObject.h"

#include <QCoreApplication>

#include <QTimer>

#include <ConsoleApplication.h>
#include <RootStorage.h>

#include <Midi/MidiToolTempo.h>

const QString MainObject::storageFileName = "timelord.bin";
const uint16_t MainObject::callbackRate = 2;

MainObject::MainObject()
   : QObject(nullptr)
   , Remember::Root()
   , polyRamps(this)
   , server(this)
   , inputDevice(this, "Metropolix_playback_1")
   , outputDevice(this, "MIDISPORT-2x2-Anniv_capture_2")
   , bridge(this, &server, &server, Midi::Device::Raspi, Midi::Device::MacBook)
#ifdef WAIT_FOR_FLAME_DEVICE
   , doepferQuad(&outputDevice)
   , quadStrips()
#endif
   , flameCC(&outputDevice)
#ifndef DO_NOT_USE_MIDI_CLOCK
   , tempo(&inputDevice)
#else  // NOT DO_NOT_USE_MIDI_CLOCK
   , tempo()
   , clockGate(&inputDevice, 10, Note::fromMidi(36))
   , resetGate(&inputDevice, 10, Note::fromMidi(37))
   , tickActive(false)
#endif // NOT DO_NOT_USE_MIDI_CLOCK
{
   qInfo() << "start";

#ifdef WAIT_FOR_FLAME_DEVICE
   for (Midi::Channel channel = 0; channel < 4; channel++)
      quadStrips[channel] = doepferQuad.create(channel + 1);
#endif

   inputDevice.open();
   //inputDevice.addPassThroughInterface(&outputDevice);

   outputDevice.open();
   flameCC.init();

   server.addPassThroughInterface(&outputDevice);
   inputDevice.addPassThroughInterface(&server);

   QTimer* loopTimer = new QTimer(this);
   loopTimer->setTimerType(Qt::PreciseTimer);
   connect(loopTimer, &QTimer::timeout, this, &MainObject::loop);
   loopTimer->setInterval(callbackRate);
   loopTimer->start();

   RootStorage fileStorage(this);
   if (fileStorage.loadFromFile(storageFileName))
      qInfo() << "restored settings";

   bridge.onPulledFromRemote(this, &MainObject::receviedSettings);
   reset();
}

MainObject::~MainObject()
{
   qInfo() << "end";
}

void MainObject::loop()
{
#ifndef DO_NOT_USE_MIDI_CLOCK
   if (tempo.isReset())
   {
      for (uint8_t index = 0; index < polyRamps.getSize(); index++)
         polyRamps[index].clockReset();
   }
   else if (tempo.isTick())
   {
      for (uint8_t index = 0; index < polyRamps.getSize(); index++)
         polyRamps[index].clockTick();
   }
   tempo.advance(callbackRate);
#else  // NOT DO_NOT_USE_MIDI_CLOCK
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
#endif // NOT DO_NOT_USE_MIDI_CLOCK

   for (uint8_t index = 0; index < polyRamps.getSize(); index++)
   {
      float voltage = 0.0;
      if (tempo.isRunningOrFirstTick())
      {
         const Tempo::Division division = polyRamps[index].getStepSize();
         const uint8_t value = polyRamps[index].getCurrentValue(tempo.getPercentage(division));
         //const uint8_t value = polyRamps[index].getCurrentValue(0);
         voltage = value * 5.0 / 255.0;
         //if(0 == index) qDebug() << voltage;
      }
#ifdef WAIT_FOR_FLAME_DEVICE
      if (index < 8)
      {
         bool isPitch = (0 == index % 2);
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

      //flameCC.setCV(index, voltage);
   }
}

void MainObject::reset()
{
#ifndef DO_NOT_USE_MIDI_CLOCK
// TODO : clock reset?
#else  // DO_NOT_USE_MIDI_CLOCK
   tempo.clockReset();
   tickActive = false;
#endif // DO_NOT_USE_MIDI_CLOCK

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
}

void MainObject::receviedSettings()
{
   reset();

   RootStorage fileStorage(this);
   if (fileStorage.saveToFile(storageFileName))
      qInfo() << "saved settings";
}

// main

int main(int argc, char** argv)
{
   qDebug() << "INPUTS:" << Midi::Physical::Input::getAvailable();
   qDebug() << "OUTPUTS" << Midi::Physical::Output::getAvailable();

   QCoreApplication app(argc, argv);
   ConsoleApplication::catchSignals();

   MainObject mo;

   return app.exec();
}
