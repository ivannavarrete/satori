
TEMPLATE = lib
CONFIG += qt plugin warn_on
QT -= gui

DESTDIR = ../../../../release/modules/arch/avr

SOURCEROOT = ../../..
INCLUDEPATH += $${SOURCEROOT}


# AVR architecture text UI
HEADERS += avrtxtui.h \
           avrcommandtable.h

SOURCES += avrtxtui.cpp \
           avrcommandtable.cpp


# Architecture-independent UI classes
HEADERS += $${SOURCEROOT}/satori/ui/txt/txtuiinterface.h \
           $${SOURCEROOT}/satori/ui/txt/memorytxtwindow.h \
		   $${SOURCEROOT}/satori/ui/txt/statetxtwindow.h

SOURCES += $${SOURCEROOT}/satori/ui/txt/memorytxtwindow.cpp \
           $${SOURCEROOT}/satori/ui/txt/statetxtwindow.cpp


# Architecture-independent device classes
HEADERS += $${SOURCEROOT}/satori/memory.h \
           $${SOURCEROOT}/satori/state.h \
           $${SOURCEROOT}/satori/commandengine.h \
		   $${SOURCEROOT}/satori/packet.h

SOURCES += $${SOURCEROOT}/satori/memory.cpp \
           $${SOURCEROOT}/satori/state.cpp \
           $${SOURCEROOT}/satori/commandengine.cpp \
		   $${SOURCEROOT}/satori/packet.cpp


# Command classes
HEADERS += $${SOURCEROOT}/lib/tree.h \
           $${SOURCEROOT}/lib/command/commandtable.h \
           $${SOURCEROOT}/lib/command/command.h \
           $${SOURCEROOT}/lib/command/argument.h \
           $${SOURCEROOT}/lib/command/wordargument.h \
           $${SOURCEROOT}/lib/command/stringargument.h \
           $${SOURCEROOT}/lib/command/numberargument.h

SOURCES += $${SOURCEROOT}/lib/command/commandtable.cpp \
           $${SOURCEROOT}/lib/command/command.cpp \
           $${SOURCEROOT}/lib/command/argument.cpp \
           $${SOURCEROOT}/lib/command/wordargument.cpp \
           $${SOURCEROOT}/lib/command/stringargument.cpp \
           $${SOURCEROOT}/lib/command/numberargument.cpp
