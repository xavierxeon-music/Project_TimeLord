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
   AudioDeviceGraph.h \
   DataCore.h \
   DelegateComboBox.h \
   DelegateSpinBox.h \
   DivisionModel.h \
   EditAbstract.h \
   EditDivision.h \
   EditLength.h \
   EditLoop.h \
   PolyRampVisu.h \
   MainWidget.h \
   PolyRampModel.h \
   PolyRampWidget.h \
   PolyLineModel.h \
   PolyLineWidget.h \
   StageModel.h \
   StageWidget.h \
   TempoWidget.h \

SOURCES += \
   AbstractWidget.cpp \
   AudioDeviceGraph.cpp \
   DataCore.cpp \
   DelegateComboBox.cpp \
   DelegateSpinBox.cpp \
   DivisionModel.cpp \
   EditAbstract.cpp \
   EditDivision.cpp \
   EditLength.cpp \
   EditLoop.cpp \
   PolyRampVisu.cpp \
   MainWidget.cpp \
   PolyRampModel.cpp \
   PolyRampWidget.cpp \
   PolyLineModel.cpp \
   PolyLineWidget.cpp \
   StageModel.cpp \
   StageWidget.cpp \
   TempoWidget.cpp \

FORMS +=

RESOURCES += \
   Icons/Icons.qrc
