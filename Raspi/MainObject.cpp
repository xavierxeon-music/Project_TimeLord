#include "MainObject.h"

#include <QCoreApplication>

#include <ConsoleApplication.h>

MainObject::MainObject()
   : QObject(nullptr)
   , server(this)
   , inputDevice("system:midi_capture_2")
   , outputDevice("system:midi_playback_2")
{
   qInfo() << "start";

   inputDevice.initMidi();
   inputDevice.addPassThroughInterface(&outputDevice);

   outputDevice.initMidi();

   server.addPassThroughInterface(&outputDevice);
   inputDevice.addPassThroughInterface(&server);
}

MainObject::~MainObject()
{
   qInfo() << "end";
}

// main

int main(int argc, char** argv)
{
   QCoreApplication app(argc, argv);
   ConsoleApplication::catchSignals();

   MainObject mo;

   return app.exec();
}
