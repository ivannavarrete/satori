
### AVR architecture text module ###

TEMPLATE = lib
CONFIG += qt plugin warn_on
QT -= gui
QT += xml

DESTDIR = ../../../../../../release/modules/arch/avr

SOURCEROOT = ../../../../..
MODULEROOT = ../..
INCLUDEPATH += $${SOURCEROOT}


# copy device files into release dir after compile, and remove after clean
QMAKE_POST_LINK += cp $${MODULEROOT}/device/* $${DESTDIR}/device/
QMAKE_CLEAN = $${DESTDIR}/device/*


# force removal of lib (currently not done with 'lib' templates)
unix:QMAKE_CLEAN += $${DESTDIR}/libavrtxt.so
win32:QMAKE_CLEAN += $${DESTDIR}/avrtxt.dll


# Architecture-dependent UI classes
HEADERS += $${MODULEROOT}/ui/txt/avrtxtui.h \
           $${MODULEROOT}/ui/txt/avrcommandtable.h

SOURCES += $${MODULEROOT}/ui/txt/avrtxtui.cpp \
           $${MODULEROOT}/ui/txt/avrcommandtable.cpp

# Architecture-dependent device classes
HEADERS += $${MODULEROOT}/avrdevice.h \
           $${MODULEROOT}/avrdeviceinfo.h

SOURCES += $${MODULEROOT}/avrdevice.cpp \
           $${MODULEROOT}/avrdeviceinfo.cpp


# Architecture-independent UI classes
HEADERS += $${SOURCEROOT}/satori/ui/txt/txtui.h \
           $${SOURCEROOT}/satori/ui/txt/memorytxtwindow.h \
		   $${SOURCEROOT}/satori/ui/txt/statetxtwindow.h

SOURCES += $${SOURCEROOT}/satori/ui/txt/memorytxtwindow.cpp \
           $${SOURCEROOT}/satori/ui/txt/statetxtwindow.cpp


# Architecture-independent device classes
HEADERS += $${SOURCEROOT}/satori/memory.h \
           $${SOURCEROOT}/satori/state.h \
           $${SOURCEROOT}/satori/commandengine.h \
		   $${SOURCEROOT}/satori/packet.h

SOURCES += $${SOURCEROOT}/satori/memory.cpp \
           $${SOURCEROOT}/satori/state.cpp \
           $${SOURCEROOT}/satori/commandengine.cpp \
		   $${SOURCEROOT}/satori/packet.cpp


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
