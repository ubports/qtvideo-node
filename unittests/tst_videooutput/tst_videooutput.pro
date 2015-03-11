TEMPLATE = app
TARGET = tst_videooutput
CONFIG += no_keywords warn_on qmltestcase
SOURCES += tst_videooutput.cpp

OTHER_FILES += $$system(ls *.qml)
