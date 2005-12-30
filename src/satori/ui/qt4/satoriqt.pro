
TEMPLATE = app
CONFIG += qt warn_on release

DEPENDPATH += .
SOURCEROOT = ../../..
INCLUDEPATH += $${SOURCEROOT}

# Input
HEADERS += widgets/widgetgallery.h \
           widgets/qxmemory.h \
           widgets/qxmemorymetrics.h \
           qxstyle.h

SOURCES += main.cpp \
           widgets/widgetgallery.cpp \
           widgets/qxmemory.cpp \
           widgets/qxmemorymetrics.cpp \
		   qxstyle.cpp


# testing
HEADERS += $${SOURCEROOT}/satori/memory.h \
           $${SOURCEROOT}/satori/commandengine.h \
		   $${SOURCEROOT}/satori/packet.h

SOURCES += $${SOURCEROOT}/satori/memory.cpp \
           $${SOURCEROOT}/satori/commandengine.cpp \
		   $${SOURCEROOT}/satori/packet.cpp
