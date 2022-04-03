#include "MainObject.h"

#include <QCoreApplication>

#include <ConsoleApplication.h>

MainObject::MainObject()
   : QObject(nullptr)
   , server(this)
   , device("system:midi_capture_2", "system:midi_playback_2")
{
   qInfo() << "start";

   device.initMidi();
   device.addPassThroughInterface(&device);

   server.addPassThroughInterface(&device);
   device.addPassThroughInterface(&server);
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
