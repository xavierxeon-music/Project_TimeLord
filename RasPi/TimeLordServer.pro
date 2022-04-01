TARGET = TimeLordServer
TEMPLATE = app

QT -= gui
CONFIG += midi
DESTDIR = ../bin

include(../../../Base/HeadlessApp.pri)

HEADERS += \
  MainObject.h \

SOURCES += \
   MainObject.cpp \
