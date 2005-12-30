
TEMPLATE = app
CONFIG += qt warn_on release

SOURCEROOT = ..
INCLUDEPATH += $${SOURCEROOT}

HEADERS += $${SOURCEROOT}/lib/tree.h

SOURCES += main.cpp

# Architecture-independent text user interface
HEADERS += ui/txt/txtuiinterface.h \
           ui/txt/basetxtui.h \
           ui/txt/basecommandtable.h

SOURCES += ui/txt/basetxtui.cpp \
           ui/txt/basecommandtable.cpp


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
