# touch.pro
# Since 7/23/2011

TEMPLATE = subdirs

include(core/core.pri)
SUBDIRS = \
    project \
    module
win32:  SUBDIRS += win
mac:    SUBDIRS += mac

OTHER_FILES = \
  ChangeLog \
  TODO

# EOF