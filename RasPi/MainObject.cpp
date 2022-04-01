#include "MainObject.h"

#include <QCoreApplication>

#include <ConsoleApplication.h>

MainObject::MainObject()
   : QObject(nullptr)
   , server(this)
   , device("blah")
{
   qDebug() << "start";
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
