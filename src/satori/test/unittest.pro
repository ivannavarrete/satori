
CONFIG += qt warn_on release
LIBS += -lcppunit
INCLUDEPATH += ../


HEADERS += signalcatcher.h

SOURCES += main.cpp \
           signalcatcher.cpp


# Serial classes
HEADERS += ../comm/comm.h \
           ../comm/serial_u.h \
           ../comm/test/serialtest.h

SOURCES += ../comm/serial_u.cpp \
           ../comm/test/serialtest.cpp


# Tree class
HEADERS += ../lib/tree.h \
		   ../lib/test/mock_tree.h \
           ../lib/test/treetest.h

SOURCES += ../lib/test/treetest.cpp


# Argument classes
HEADERS += ../ui/txt/argument.h \
           ../ui/txt/test/argumenttest.h \
           ../ui/txt/numberargument.h \
           ../ui/txt/test/numberargumenttest.h \
           ../ui/txt/stringargument.h \
           ../ui/txt/test/stringargumenttest.h \
           ../ui/txt/wordargument.h \
           ../ui/txt/test/wordargumenttest.h

SOURCES += ../ui/txt/argument.cpp \
           ../ui/txt/test/argumenttest.cpp \
           ../ui/txt/numberargument.cpp \
           ../ui/txt/test/numberargumenttest.cpp \
           ../ui/txt/stringargument.cpp \
           ../ui/txt/test/stringargumenttest.cpp \
           ../ui/txt/wordargument.cpp \
           ../ui/txt/test/wordargumenttest.cpp


HEADERS += ../ui/txt/command.h \
           ../ui/txt/test/commandtest.h

SOURCES += ../ui/txt/command.cpp \
           ../ui/txt/test/commandtest.cpp


HEADERS += ../ui/txt/commandtable.h \
           ../ui/txt/test/commandtabletest.h \
           ../ui/txt/basecommandtable.h

SOURCES += ../ui/txt/commandtable.cpp \
           ../ui/txt/test/commandtabletest.cpp \
           ../ui/txt/basecommandtable.cpp
