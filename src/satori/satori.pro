
TEMPLATE = app
CONFIG += qt warn_on

DESTDIR = ../../release/bin

SOURCEROOT = ..
INCLUDEPATH += $${SOURCEROOT}


RESOURCES = resources.qrc
FORMS = ui/qt4/forms/mainwindow.ui


# Misc source files
HEADERS += $${SOURCEROOT}/lib/tree.h

SOURCES += main.cpp


# Architecture-independent text user interface
HEADERS += ui/txt/txtmain.h \
           ui/txt/txtuiinterface.h \
           ui/txt/basetxtui.h \
           ui/txt/basecommandtable.h

SOURCES += ui/txt/txtmain.cpp \
           ui/txt/basetxtui.cpp \
           ui/txt/basecommandtable.cpp


# Architecture-independent Qt user interface
HEADERS += ui/qt4/qtmain.h \
           ui/qt4/qxmainwindow.h \
           ui/qt4/widgets/widgetgallery.h \
           ui/qt4/widgets/qxmemory.h \
           ui/qt4/widgets/qxmemorymetrics.h \
           ui/qt4/qxstyle.h

SOURCES += ui/qt4/qtmain.cpp \
           ui/qt4/qxmainwindow.cpp \
           ui/qt4/widgets/widgetgallery.cpp \
           ui/qt4/widgets/qxmemory.cpp \
           ui/qt4/widgets/qxmemorymetrics.cpp \
           ui/qt4/qxstyle.cpp

# testing
HEADERS += $${SOURCEROOT}/satori/memory.h \
           $${SOURCEROOT}/satori/commandengine.h \
		   $${SOURCEROOT}/satori/packet.h

SOURCES += $${SOURCEROOT}/satori/memory.cpp \
           $${SOURCEROOT}/satori/commandengine.cpp \
		   $${SOURCEROOT}/satori/packet.cpp


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
