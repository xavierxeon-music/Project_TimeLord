TARGET = TimeLord
TEMPLATE = app

include(../../../Base/PatchImposterApp.pri)
include(../Common/Common.pri)

HEADERS += \
    #Icons.h \
    Main.h \
    PageOverview.h

SOURCES += \
    #Icons.cpp \
    #IconsData.cpp \
    Main.cpp \
    PageOverview.cpp
