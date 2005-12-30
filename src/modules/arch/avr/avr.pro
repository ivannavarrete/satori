
TEMPLATE = lib
CONFIG += qt plugin warn_on release
QT -= gui


SOURCE_ROOT = ../../..
INCLUDEPATH += $${SOURCE_ROOT}


# AVR architecture text UI
HEADERS += avrtxtui.h \
           avrcommandtable.h

SOURCES += avrtxtui.cpp \
           avrcommandtable.cpp


# Architecture-independent UI classes
HEADERS += $${SOURCE_ROOT}/satori/ui/txt/txtuiinterface.h \
           $${SOURCE_ROOT}/satori/ui/txt/memorytxtwindow.h

SOURCES += $${SOURCE_ROOT}/satori/ui/txt/memorytxtwindow.cpp


# Architecture-independent device classes
HEADERS += $${SOURCE_ROOT}/satori/memory.h \
           $${SOURCE_ROOT}/satori/commandengine.h \
		   $${SOURCE_ROOT}/satori/packet.h

SOURCES += $${SOURCE_ROOT}/satori/memory.cpp \
           $${SOURCE_ROOT}/satori/commandengine.cpp \
		   $${SOURCE_ROOT}/satori/packet.cpp


# Command classes
HEADERS += $${SOURCE_ROOT}/lib/tree.h \
           $${SOURCE_ROOT}/lib/command/commandtable.h \
           $${SOURCE_ROOT}/lib/command/command.h \
           $${SOURCE_ROOT}/lib/command/argument.h \
           $${SOURCE_ROOT}/lib/command/wordargument.h \
           $${SOURCE_ROOT}/lib/command/stringargument.h \
           $${SOURCE_ROOT}/lib/command/numberargument.h

SOURCES += $${SOURCE_ROOT}/lib/command/commandtable.cpp \
           $${SOURCE_ROOT}/lib/command/command.cpp \
           $${SOURCE_ROOT}/lib/command/argument.cpp \
           $${SOURCE_ROOT}/lib/command/wordargument.cpp \
           $${SOURCE_ROOT}/lib/command/stringargument.cpp \
           $${SOURCE_ROOT}/lib/command/numberargument.cpp
