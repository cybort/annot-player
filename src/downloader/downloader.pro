# downloader.pro
# 3/29/2012

VERSION = 0.1.2.5

DEFINES += SRC_DOWNLOADER

include(../../config.pri)
include($$ROOTDIR/src/common/common.pri)

## Libraries

include($$ROOTDIR/lib/animation/animation.pri)
include($$ROOTDIR/lib/annotcache/annotcache.pri)
include($$ROOTDIR/lib/annotcloud/annotcloud.pri)
include($$ROOTDIR/lib/annotdown/annotdown.pri)
include($$ROOTDIR/lib/annotservice/annotservice.pri)
include($$ROOTDIR/lib/compress/compress.pri)
include($$ROOTDIR/lib/crypt/crypt.pri)
include($$ROOTDIR/lib/download/download.pri)
include($$ROOTDIR/lib/downloadtask/downloadtask.pri)
include($$ROOTDIR/lib/gsoap/gsoap.pri)
include($$ROOTDIR/lib/htmlutil/htmlutil.pri)
include($$ROOTDIR/lib/imagefilter/imagefilter.pri)
include($$ROOTDIR/lib/mediacodec/mediacodec.pri)
include($$ROOTDIR/lib/mrlresolver/mrlresolver.pri)
include($$ROOTDIR/lib/mstypes/mstypes.pri)
include($$ROOTDIR/lib/qt/qt.pri)
include($$ROOTDIR/lib/qtx/qtx.pri)
include($$ROOTDIR/lib/searchengine/searchengine.pri)
include($$ROOTDIR/lib/stream/stream.pri)

!mac: include($$ROOTDIR/lib/metacall/metacall.pri)

win32 {
    include($$ROOTDIR/win/dwm/dwm.pri)
    include($$ROOTDIR/win/qtwin/qtwin.pri)
}
unix: {
    include($$ROOTDIR/unix/qtunix/qtunix.pri)
}
mac {
    include($$ROOTDIR/mac/qtmac/qtmac.pri)
    include($$ROOTDIR/mac/qtcocoa/qtcocoa.pri)
}

win32 {
  #DEFINES += AC_ENABLE_GAME
  #DEFINES += AC_ENABLE_LAUNCHER
  DEFINES += AC_ENABLE_UPDATE
}

QT      += core gui network xml

## Sources

TEMPLATE      = app
win32:  TARGET = annot-down
unix:   TARGET = annot-down
mac:   TARGET = "Annot Downloader"

win32: CONFIG += windows

HEADERS += \
    application.h \
    clipboardmonitor.h \
    global.h \
    mainwindow.h \
    mainwindow_p.h \
    rc.h \
    settings.h \
    signer.h \
    taskdialog.h \
    trayicon.h

SOURCES += \
    application.cc \
    clipboardmonitor.cc \
    main.cc \
    mainwindow.cc \
    settings.cc \
    signer.cc \
    taskdialog.cc \
    trayicon.cc

RESOURCES += downloader.qrc

OTHER_FILES += \
    annot-down.desktop \
    annot-down.png \
    Info.plist \
    debian.control \
    debian.rules \
    deploy-debian.sh \
    deploy-fedora.sh \
    downloader.rc \
    downloader.ico \
    downloader.icns \
    rpm.spec

win32 {
    !wince*: LIBS += -lshell32
    RC_FILE += downloader.rc
}

mac {
    ICON = downloader.icns
    QMAKE_INFO_PLIST = Info.plist
}

## Translations

TRANSLATIONS += \
    $$PWD/tr/tr_ja.ts \
    $$PWD/tr/tr_zh_TW.ts \
    $$PWD/tr/tr_zh_CN.ts

CODECFORTR = UTF-8

OTHER_FILES += $$TRANSLATIONS \
    $$PWD/tr/lrelease_tr.cmd \
    $$PWD/tr/lrelease_tr.sh \
    $$PWD/tr/lupdate_tr.cmd \
    $$PWD/tr/lupdate_tr.sh

# Deployment

unix:!mac {
    INSTALLS += target desktop desktop-kde icon lua #avatar

    target.path = $$BINDIR

    desktop.path = $$DATADIR/applications
    desktop.files += $${TARGET}.desktop

    desktop-kde.path = $$DATADIR/kde4/apps/solid/actions
    desktop-kde.files += $${TARGET}.desktop

    #service.path = $$DATADIR/dbus-1/services
    #service.files += $${TARGET}.service

    icon.path = $$DATADIR/icons/hicolor/256x256/apps
    icon.files += $${TARGET}.png

    LUADIR = $$DATADIR/annot/down/lua
    lua.path = $$LUADIR
    lua.files = $$LUA_FILES

    AVATARDIR = $$DATADIR/annot/downloader/avatars
    avatar.path = $$AVATARDIR
    avatar.files = $$AVATAR_FILES
}

# EOF
