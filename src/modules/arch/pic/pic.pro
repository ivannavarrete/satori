
TEMPLATE = lib
CONFIG += qt plugin warn_on release


INCLUDEPATH += ../../../ui/txt/pic

HEADERS += ../../../lib/tree.h

# Architecture-independent text user interface
HEADERS += ../../../ui/txt/commandtable.h

SOURCES += ../../../ui/txt/commandtable.cpp


# AVR architecture plugin
HEADERS += pictxtui.h \
           piccommandtable.h

SOURCES += pictxtui.cpp \
           piccommandtable.cpp


# Command classes
HEADERS += ../../../ui/txt/command.h \
           ../../../ui/txt/argument.h \
           ../../../ui/txt/wordargument.h \
           ../../../ui/txt/stringargument.h \
           ../../../ui/txt/numberargument.h

SOURCES += ../../../ui/txt/command.cpp \
           ../../../ui/txt/argument.cpp \
           ../../../ui/txt/wordargument.cpp \
           ../../../ui/txt/stringargument.cpp \
           ../../../ui/txt/numberargument.cpp
