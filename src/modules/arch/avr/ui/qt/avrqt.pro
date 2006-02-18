
### AVR architecture Qt module ###

TEMPLATE = lib
CONFIG += qt plugin warn_on

DESTDIR = ../../../../../../release/modules/arch/avr

SOURCEROOT = ../../../../..
MODULEROOT = ../..
INCLUDEPATH += $${SOURCEROOT}


# copy device files into release dir after compile, and remove after clean
QMAKE_POST_LINK += cp $${MODULEROOT}/device/* $${DESTDIR}/device/
QMAKE_CLEAN = $${DESTDIR}/device/*


# force removal of lib (currently not done with 'lib' templates)
unix:QMAKE_CLEAN = $${DESTDIR}/libavrqt.so
win32:QMAKE_CLEAN = $${DESTDIR}/avrqt.dll


# AVR architecture text UI
HEADERS += avrqtui.h

SOURCES += avrqtui.cpp


# Architecture-independent UI classes
HEADERS += $${SOURCEROOT}/satori/ui/qt/qtui.h
