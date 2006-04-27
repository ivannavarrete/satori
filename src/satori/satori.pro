
TEMPLATE = app
CONFIG += qt warn_on

DESTDIR = ../../release/bin

SOURCEROOT = ..
INCLUDEPATH += $${SOURCEROOT}


RESOURCES = resources.qrc
FORMS = ui/qt/forms/mainwindow.ui


# Misc source files
HEADERS += $${SOURCEROOT}/lib/tree.h

SOURCES += main.cpp


# Architecture-independent text user interface
HEADERS += ui/txt/txtmain.h \
           ui/txt/txtui.h \
           ui/txt/basetxtui.h \
           ui/txt/basecommandtable.h \
           ui/txt/color.h

SOURCES += ui/txt/txtmain.cpp \
           ui/txt/basetxtui.cpp \
           ui/txt/basecommandtable.cpp \
           ui/txt/color.cpp


# Architecture-independent Qt user interface
HEADERS += ui/qt/qtmain.h \
           ui/qt/qtui.h \
           ui/qt/qxmainwindow.h \
           ui/qt/qxstyle.h

SOURCES += ui/qt/qtmain.cpp \
           ui/qt/qxmainwindow.cpp \
           ui/qt/qxstyle.cpp


# Command classes
HEADERS += $${SOURCEROOT}/lib/command/commandtable.h \
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
