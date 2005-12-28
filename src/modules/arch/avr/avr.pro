
TEMPLATE = lib
CONFIG += qt plugin warn_on release
QT -= gui


SOURCE_ROOT = ../../..
INCLUDEPATH += $${SOURCE_ROOT}


# AVR architecture text UI
HEADERS += $${SOURCE_ROOT}/ui/txt/txtuiinterface.h \
           avrtxtui.h \
           avrcommandtable.h

SOURCES += avrtxtui.cpp \
           avrcommandtable.cpp


# Architecture-independent UI classes
HEADERS += $${SOURCE_ROOT}/ui/txt/memorytxtwindow.h

SOURCES += $${SOURCE_ROOT}/ui/txt/memorytxtwindow.cpp


# Architecture-independent device classes
HEADERS += $${SOURCE_ROOT}/memory.h \
           $${SOURCE_ROOT}/commandengine.h \
		   $${SOURCE_ROOT}/packet.h

SOURCES += $${SOURCE_ROOT}/memory.cpp \
           $${SOURCE_ROOT}/commandengine.cpp \
		   $${SOURCE_ROOT}/packet.cpp


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
