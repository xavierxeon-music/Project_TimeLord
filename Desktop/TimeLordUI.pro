TARGET = TimeLordUI
TEMPLATE = app

QT += widgets svg
CONFIG += midi
DESTDIR = ../bin

win32:RC_ICONS = ApplicaionIcon/TimeLord.ico
macos:ICON = ApplicaionIcon/TimeLord.icns

include(../../../Base/DesktopApp.pri)

HEADERS += \
   AbstractEdit.h \
   AbstractWidget.h \
   BankContent.h \
   BankModel.h \
   BankWidget.h \
   Core.h \
   Core.hpp \
   DelegateComboBox.h \
   DelegateSpinBox.h \
   DeviceStateModel.h \
   DeviceStateWidget.h \
   EditDivision.h \
   EditLength.h \
   EditLoop.h \
   MainWidget.h \
   ModelDivision.h \
   RampModel.h \
   RampWidget.h \
   StageModel.h \
   StageWidget.h \
   Target.h \
   VisuWidget.h

SOURCES += \
   AbstractEdit.cpp \
   AbstractWidget.cpp \
   BankContent.cpp \
   BankModel.cpp \
   BankWidget.cpp \
   Core.cpp \
   DelegateComboBox.cpp \
   DelegateSpinBox.cpp \
   DeviceStateModel.cpp \
   DeviceStateWidget.cpp \
   EditDivision.cpp \
   EditLength.cpp \
   EditLoop.cpp \
   MainWidget.cpp \
   ModelDivision.cpp \
   RampModel.cpp \
   RampWidget.cpp \
   StageModel.cpp \
   StageWidget.cpp \
   Target.cpp \
   VisuWidget.cpp


RESOURCES += \
   Icons/Icons.qrc

FORMS += \
   DeviceStateWidget.ui
