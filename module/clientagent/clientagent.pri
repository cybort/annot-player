# clientdelegate.pri
# Since 10/6/2011

include(../../config.pri)
#include($$ROOTDIR/module/gsoap/gsoap.pri)
#include($$ROOTDIR/core/core.pri)
#include($$PWD/clientsoap.pri)

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/clientsoap

#DEFINES += USE_MODULE_SERVERAGENT

HEADERS += \
    $$PWD/clientagent.h \
    $$PWD/clientdelegate.h \
    $$PWD/clientservice.h \
    $$PWD/clientservice_config.h \
    $$PWD/clientsoap.h

SOURCES += \
    $$PWD/clientagent.cc \
    $$PWD/clientservice.cc \
    $$PWD/clientsoap.cc

OTHER_FILES += \
    $$PWD/wsdl/clientagent.wsdl \
    $$PWD/wsdl/clientagent.xsd

QT      += core

# EOF