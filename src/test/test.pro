
CONFIG += qt warn_on
LIBS += -lcppunit

SOURCEROOT = ..
INCLUDEPATH += $${SOURCEROOT}

QMAKE_POST_LINK = ./$${TARGET}


# Main unit test files
HEADERS += suitename.h

SOURCES += main.cpp \
		   suitename.cpp


## Serial classes
#HEADERS += ../comm/comm.h \
#           ../comm/serial_u.h \
#           ../comm/test/serialtest.h
#
#SOURCES += ../comm/serial_u.cpp \
#           ../comm/test/serialtest.cpp


# Tree class
HEADERS += $${SOURCEROOT}/lib/tree.h \
		   $${SOURCEROOT}/lib/test/mock_tree.h \
           $${SOURCEROOT}/lib/test/mocknode.h \
           $${SOURCEROOT}/lib/test/treetest.h

SOURCES += $${SOURCEROOT}/lib/test/treetest.cpp


# Command test suite
HEADERS += $${SOURCEROOT}/lib/command/command.h \
           $${SOURCEROOT}/lib/command/test/commandtest.h \
		   $${SOURCEROOT}/lib/command/commandtable.h \
		   $${SOURCEROOT}/lib/command/test/commandtabletest.h \
           $${SOURCEROOT}/lib/command/argument.h \
           $${SOURCEROOT}/lib/command/test/argumenttest.h \
           $${SOURCEROOT}/lib/command/numberargument.h \
           $${SOURCEROOT}/lib/command/test/numberargumenttest.h \
           $${SOURCEROOT}/lib/command/stringargument.h \
           $${SOURCEROOT}/lib/command/test/stringargumenttest.h \
           $${SOURCEROOT}/lib/command/wordargument.h \
           $${SOURCEROOT}/lib/command/test/wordargumenttest.h

SOURCES += $${SOURCEROOT}/lib/command/command.cpp \
           $${SOURCEROOT}/lib/command/test/commandtest.cpp \
		   $${SOURCEROOT}/lib/command/commandtable.cpp \
		   $${SOURCEROOT}/lib/command/test/commandtabletest.cpp \
           $${SOURCEROOT}/lib/command/argument.cpp \
           $${SOURCEROOT}/lib/command/test/argumenttest.cpp \
           $${SOURCEROOT}/lib/command/numberargument.cpp \
           $${SOURCEROOT}/lib/command/test/numberargumenttest.cpp \
           $${SOURCEROOT}/lib/command/stringargument.cpp \
           $${SOURCEROOT}/lib/command/test/stringargumenttest.cpp \
           $${SOURCEROOT}/lib/command/wordargument.cpp \
           $${SOURCEROOT}/lib/command/test/wordargumenttest.cpp
