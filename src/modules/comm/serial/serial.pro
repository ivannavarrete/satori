
TEMPLATE = lib
CONFIG += qt plugin warn_on
QT -= gui

DESTDIR = ../../../../release/modules/comm/serial

SOURCEROOT = ../../..
INCLUDEPATH += $${SOURCEROOT}


# Comm independent UI classes
HEADERS += $${SOURCEROOT}/satori/ui/txt/txtuiinterface.h

# Serial communication text UI
HEADERS += txt/serialtxtui.h \
           txt/serialcommandtable.h

SOURCES += txt/serialtxtui.cpp \
           txt/serialcommandtable.cpp


# Serial device classes
win32 {
	HEADERS += serial_w.h
	SOURCES += serial_w.cpp
}

unix {
	HEADERS += serial_u.h
	SOURCES += serial_u.cpp
}


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
