TEMPLATE = app
TARGET = tst_videooutput
CONFIG += no_keywords warn_on qmltestcase
SOURCES += tst_videooutput.cpp

OTHER_FILES += $$system(ls *.qml)

check.commands = "xvfb-run -a -s '-screen 0 1024x768x24'"
check.depends = $${TARGET}
QMAKE_EXTRA_TARGETS += check
