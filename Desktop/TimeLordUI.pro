TARGET = TimeLordUI
TEMPLATE = app

QT += widgets svg
CONFIG += audio midi
DESTDIR = ../bin

include(../../../Base/DesktopApp.pri)

HEADERS += \
   AbstractWidget.h \
   DataCore.h \
   DeviceModel.h \
   GraphAudioDevice.h \
   GraphWidget.h \
   MainWidget.h \
   PointModel.h \
   PointWidget.h \
   DeviceWidget.h

SOURCES += \
   AbstractWidget.cpp \
   DataCore.cpp \
   DeviceModel.cpp \
   GraphAudioDevice.cpp \
   GraphWidget.cpp \
   MainWidget.cpp \
   PointModel.cpp \
   PointWidget.cpp \
   DeviceWidget.cpp

FORMS +=

RESOURCES += \
   Icons/Icons.qrc

