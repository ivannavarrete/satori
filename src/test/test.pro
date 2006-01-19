
CONFIG += qt warn_on
LIBS += -lcppunit

SOURCEROOT = ..
INCLUDEPATH += $${SOURCEROOT}

QMAKE_POST_LINK = ./$${TARGET}


# Main unit test files
HEADERS += suitename.h \
           signalcatcher.h

SOURCES += main.cpp \
		   suitename.cpp \
           signalcatcher.cpp


# Tree class test suite
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
           $${SOURCEROOT}/lib/command/test/wordargumenttest.h \
		   $${SOURCEROOT}/lib/command/characterargument.h \
		   $${SOURCEROOT}/lib/command/test/characterargumenttest.h

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
           $${SOURCEROOT}/lib/command/test/wordargumenttest.cpp \
		   $${SOURCEROOT}/lib/command/characterargument.cpp \
		   $${SOURCEROOT}/lib/command/test/characterargumenttest.cpp


# Serial test suite
HEADERS += $${SOURCEROOT}/modules/comm/comm.h \
           $${SOURCEROOT}/modules/comm/serial/serial_u.h \
           $${SOURCEROOT}/modules/comm/serial/test/serialtest.h

SOURCES += $${SOURCEROOT}/modules/comm/serial/serial_u.cpp \
           $${SOURCEROOT}/modules/comm/serial/test/serialtest.cpp

