
TEMPLATE = lib
CONFIG += qt plugin warn_on
QT -= gui

DESTDIR = ../../../../release/modules/comm/serial

SOURCEROOT = ../../..
INCLUDEPATH += $${SOURCEROOT}

# force removal of lib (currently not done with 'lib' templates)
unix:QMAKE_CLEAN = $${DESTDIR}/libserial.so
win32:QMAKE_CLEAN = $${DESTDIR}/serial.dll


# Comm independent UI classes
HEADERS += $${SOURCEROOT}/satori/ui/txt/txtui.h \
           $${SOURCEROOT}/satori/ui/txt/color.h

SOURCES += $${SOURCEROOT}/satori/ui/txt/color.cpp

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
