TARGET = shadervideonode
QT += multimedia-private qtmultimediaquicktools-private opengl

include(../coverage.pri)

PLUGIN_TYPE = video/videonode

load(qt_plugin)
target.path += $$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS = target

INCLUDEPATH += /usr/include/hybris/camera /usr/include/hybris/media
LIBS += -lhybris_ics -lcamera -lmediaplayer

OTHER_FILES += shadervideonode.json

HEADERS += \
    shadervideonodeplugin.h \
    shadervideomaterial.h \
    shadervideoshader.h \
    shadervideonode.h

SOURCES += \
    shadervideonodeplugin.cpp \
    shadervideomaterial.cpp \
    shadervideoshader.cpp \
    shadervideonode.cpp
