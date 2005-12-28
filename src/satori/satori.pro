
TEMPLATE = app
CONFIG += qt warn_on release

HEADERS += lib/tree.h

SOURCES += main.cpp

# Architecture-independent text user interface
HEADERS += ui/txt/txtuiinterface.h \
           ui/txt/basetxtui.h \
           ui/txt/basecommandtable.h

SOURCES += ui/txt/basetxtui.cpp \
           ui/txt/basecommandtable.cpp


# Command classes
HEADERS += lib/command/commandtable.h \
           lib/command/command.h \
           lib/command/argument.h \
           lib/command/wordargument.h \
           lib/command/stringargument.h \
           lib/command/numberargument.h

SOURCES += lib/command/commandtable.cpp \
           lib/command/command.cpp \
           lib/command/argument.cpp \
           lib/command/wordargument.cpp \
           lib/command/stringargument.cpp \
           lib/command/numberargument.cpp


#HEADERS += commandengine.h \
#           memory.h \
#           packet.h \
#           comm/comm.h \
#           comm/serial.h \
#           ui/txt/uitxt.h \
#           ui/txt/commandshell.h \
#           ui/txt/commandtable.h \
#           ui/txt/command.h \
#           ui/txt/argument.h \
#           ui/txt/memoryw.h \
#           ui/txt/avr/avruitxt.h \
#           ui/txt/avr/avrcommandtable.h
#
#SOURCES += main.cpp \
#           memory.cpp \
#           commandengine.cpp \
#           packet.cpp \
#           ui/txt/uitxt.cpp \
#           ui/txt/commandshell.cpp \
#           ui/txt/commandtable.cpp \
#           ui/txt/command.cpp \
#           ui/txt/argument.cpp \
#           ui/txt/memoryw.cpp \
#           ui/txt/avr/avruitxt.cpp \
#           ui/txt/avr/avrcommandtable.cpp
#
#
#win32 {
#    HEADERS += comm/serial_w.h
#    SOURCES += comm/serial_w.cpp
#}
#
#unix {
#    HEADERS += comm/serial_u.h
#    SOURCES += comm/serial_u.cpp
#}
