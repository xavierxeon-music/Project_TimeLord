TARGET = TimeLordUI
TEMPLATE = app

QT += widgets svg
CONFIG += audio midi
DESTDIR = ../bin

include(../../../Base/DesktopApp.pri)

HEADERS += \
   AbstractWidget.h \
   DelegateComboBox.h \
   DataCore.h \
   DivisionModel.h \
   GraphAudioDevice.h \
   GraphModel.h \
   GraphVisuWidget.h \
   GraphWidget.h \
   MainWidget.h \
   DelegateSpinBox.h \
   StageModel.h \
   StageWidget.h \

SOURCES += \
   AbstractWidget.cpp \
   DelegateComboBox.cpp \
   DataCore.cpp \
   DivisionModel.cpp \
   GraphAudioDevice.cpp \
   GraphModel.cpp \
   GraphVisuWidget.cpp \
   GraphWidget.cpp \
   MainWidget.cpp \
   DelegateSpinBox.cpp \
   StageModel.cpp \
   StageWidget.cpp \

FORMS +=

RESOURCES += \
   Icons/Icons.qrc
