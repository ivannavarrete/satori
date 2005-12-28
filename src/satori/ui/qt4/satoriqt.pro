
TEMPLATE = app
CONFIG += qt warn_on release

DEPENDPATH += .
SOURCE_ROOT = ../..
INCLUDEPATH += $${SOURCE_ROOT}

# Input
HEADERS += widgets/widgetgallery.h \
           widgets/qxmemory.h \
           qxstyle.h

SOURCES += main.cpp \
           widgets/widgetgallery.cpp \
           widgets/qxmemory.cpp \
		   qxstyle.cpp


# testing
HEADERS += $${SOURCE_ROOT}/memory.h \
           $${SOURCE_ROOT}/commandengine.h \
		   $${SOURCE_ROOT}/packet.h

SOURCES += $${SOURCE_ROOT}/memory.cpp \
           $${SOURCE_ROOT}/commandengine.cpp \
		   $${SOURCE_ROOT}/packet.cpp
