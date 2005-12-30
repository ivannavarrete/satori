
TEMPLATE = lib
CONFIG += qt plugin warn_on release
QT -= gui


SOURCE_ROOT = ../../..
INCLUDEPATH += $${SOURCE_ROOT}


# Comm independent UI classes
HEADERS += $${SOURCE_ROOT}/satori/ui/txt/txtuiinterface.h \

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
