TARGET = TimeLordUI
TEMPLATE = app

QT += widgets svg
CONFIG += audio midi
DESTDIR = ..

include(../../../Base/SeedCompanionApp.pri)
include(../Common/Common.pri)

HEADERS += \
   DataCore.h \
   GraphDevice.h \
   GraphWidget.h \
   MainWidget.h \
   PointModel.h \
   PointWidget.h \
   PortModel.h \
   PortWidget.h

SOURCES += \
   DataCore.cpp \
   GraphDevice.cpp \
   GraphWidget.cpp \
   MainWidget.cpp \
   PointModel.cpp \
   PointWidget.cpp \
   PortModel.cpp \
   PortWidget.cpp

FORMS +=

RESOURCES += \
   Icons/Icons.qrc

