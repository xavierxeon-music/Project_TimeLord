#include "MainObject.h"

#include <QCoreApplication>

#include <QTimer>

#include <ConsoleApplication.h>

MainObject::MainObject()
   : QObject(nullptr)
   , Remember::Root()
   , polyRamps(this)
   , server(this)
   , inputDevice("system:midi_capture_2")
   , outputDevice("system:midi_playback_2")
   , doepferQuad(&outputDevice)
   , flameCC(&outputDevice)
   , tempo()
{
   qInfo() << "start";

   inputDevice.initMidi();
   inputDevice.addPassThroughInterface(&outputDevice);

   outputDevice.initMidi();
   flameCC.init();

   server.addPassThroughInterface(&outputDevice);
   inputDevice.addPassThroughInterface(&server);
}

MainObject::~MainObject()
{
   qInfo() << "end";
}

void MainObject::loop()
{
}

void MainObject::clockTick()
{
}

void MainObject::clockReset()
{
}

// main

int main(int argc, char** argv)
{
   QCoreApplication app(argc, argv);
   ConsoleApplication::catchSignals();

   MainObject mo;

   return app.exec();
}
