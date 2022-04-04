TARGET = TimeLordUI
TEMPLATE = app

QT += widgets svg
CONFIG += audio midi
DESTDIR = ../bin


win32:RC_ICONS = ApplicaionIcon/TimeLord.ico
macos:ICON = ApplicaionIcon/TimeLord.icns

include(../../../Base/DesktopApp.pri)

HEADERS += \
   AbstractEdit.h \
   AbstractWidget.h \
   DataCore.h \
   DelegateComboBox.h \
   DelegateSpinBox.h \
   DivisionModel.h \
   EditDivision.h \
   EditLength.h \
   EditLoop.h \
   MainWidget.h \
   PolyLineModel.h \
   PolyLineWidget.h \
   RampDeviceRaspi.h \
   RampModel.h \
   RampVisu.h \
   RampWidget.h \
   StageModel.h \
   StageWidget.h \
   TempoWidget.h \

SOURCES += \
   AbstractEdit.cpp \
   AbstractWidget.cpp \
   DataCore.cpp \
   DelegateComboBox.cpp \
   DelegateSpinBox.cpp \
   DivisionModel.cpp \
   EditDivision.cpp \
   EditLength.cpp \
   EditLoop.cpp \
   MainWidget.cpp \
   PolyLineModel.cpp \
   PolyLineWidget.cpp \
   RampDeviceRaspi.cpp \
   RampModel.cpp \
   RampVisu.cpp \
   RampWidget.cpp \
   StageModel.cpp \
   StageWidget.cpp \
   TempoWidget.cpp \

FORMS +=

RESOURCES += \
   Icons/Icons.qrc
