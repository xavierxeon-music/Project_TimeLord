#include "MainObject.h"

#include <QCoreApplication>

#include <ConsoleApplication.h>

MainObject::MainObject()
   : QObject(nullptr)
   , server(this)
   , device("system:midi_capture_2", "system:midi_playback_2", &device)
{
   qDebug() << "start";

   device.initMidi(true);
}

MainObject::~MainObject()
{
   qDebug() << "end";
}

// main

int main(int argc, char** argv)
{
   QCoreApplication app(argc, argv);
   ConsoleApplication::catchSignals();

   MainObject mo;

   return app.exec();
}
