TARGET = TimeLordUI
TEMPLATE = app

QT += widgets svg
CONFIG += audio midi
DESTDIR = ../bin


win32:RC_ICONS = ApplicaionIcon/TimeLord.ico
macos:ICON = ApplicaionIcon/TimeLord.icns

include(../../../Base/DesktopApp.pri)

HEADERS += \
   AbstractWidget.h \
   DelegateComboBox.h \
   DataCore.h \
   DivisionModel.h \
   GraphAudioDevice.h \
   GraphEditAbstract.h \
   GraphEditDivision.h \
   GraphEditLength.h \
   GraphEditLoop.h \
   GraphModel.h \
   GraphVisuWidget.h \
   GraphWidget.h \
   MainWidget.h \
   DelegateSpinBox.h \
   StageModel.h \
   StageWidget.h \
   TempoWidget.h

SOURCES += \
   AbstractWidget.cpp \
   DelegateComboBox.cpp \
   DataCore.cpp \
   DivisionModel.cpp \
   GraphAudioDevice.cpp \
   GraphEditAbstract.cpp \
   GraphEditDivision.cpp \
   GraphEditLength.cpp \
   GraphEditLoop.cpp \
   GraphModel.cpp \
   GraphVisuWidget.cpp \
   GraphWidget.cpp \
   MainWidget.cpp \
   DelegateSpinBox.cpp \
   StageModel.cpp \
   StageWidget.cpp \
   TempoWidget.cpp

FORMS +=

RESOURCES += \
   Icons/Icons.qrc
