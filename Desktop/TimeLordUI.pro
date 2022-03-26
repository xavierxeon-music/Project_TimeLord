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
   GraphAudioDevice.h \
   GraphModel.h \
   GraphVisuWidget.h \
   GraphWidget.h \
   MainWidget.h \
   SpinBoxDelegate.h \
   StageModel.h \
   StageWidget.h \

SOURCES += \
   AbstractWidget.cpp \
   ComboBoxDelegate.cpp \
   DataCore.cpp \
   GraphAudioDevice.cpp \
   GraphModel.cpp \
   GraphVisuWidget.cpp \
   GraphWidget.cpp \
   MainWidget.cpp \
   SpinBoxDelegate.cpp \
   StageModel.cpp \
   StageWidget.cpp \

FORMS +=

RESOURCES += \
   Icons/Icons.qrc

