# metacall.pri
# 4/9/2012

DEFINES += WITH_LIB_METACALL

DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/metacallevent.h \
    $$PWD/metacallfilter.h \
    $$PWD/metacallpropagator.h
SOURCES += \
    $$PWD/metacallfilter.cc \
    $$PWD/metacallpropagator.cc

QT += core network

#INCLUDEPATH += $$QT_SRC

# EOF
