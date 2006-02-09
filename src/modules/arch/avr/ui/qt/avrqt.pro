
### AVR architecture Qt module ###

TEMPLATE = lib
CONFIG += qt plugin warn_on

DESTDIR = ../../../../../../release/modules/arch/avr

SOURCEROOT = ../../../../..
INCLUDEPATH += $${SOURCEROOT}

# force removal of lib (currently not done with 'lib' templates)
unix:QMAKE_CLEAN = $${DESTDIR}/libavrqt.so
win32:QMAKE_CLEAN = $${DESTDIR}/avrqt.dll


# AVR architecture text UI
HEADERS += avrqtui.h

SOURCES += avrqtui.cpp


# Architecture-independent UI classes
HEADERS += $${SOURCEROOT}/satori/ui/qt/qtui.h
