TARGET = TimeLordUI
TEMPLATE = app

QT += widgets svg
CONFIG += audio midi
DESTDIR = ../bin

include(../../../Base/DesktopApp.pri)

HEADERS += \
   AbstractWidget.h \
   ComboBoxDelegate.h \
   DataCore.h \
   DeviceModel.h \
   GraphAudioDevice.h \
   GraphWidget.h \
   MainWidget.h \
   PointModel.h \
   PointWidget.h \
   DeviceWidget.h \
   SpinBoxDelegate.h

SOURCES += \
   AbstractWidget.cpp \
   ComboBoxDelegate.cpp \
   DataCore.cpp \
   DeviceModel.cpp \
   GraphAudioDevice.cpp \
   GraphWidget.cpp \
   MainWidget.cpp \
   PointModel.cpp \
   PointWidget.cpp \
   DeviceWidget.cpp \
   SpinBoxDelegate.cpp

FORMS +=

RESOURCES += \
   Icons/Icons.qrc

