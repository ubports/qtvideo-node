TARGET = shadervideonode
QT += multimedia-private qtmultimediaquicktools-private opengl

include(../coverage.pri)

PLUGIN_TYPE = video/videonode

load(qt_plugin)
target.path += $$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS = target

INCLUDEPATH += /usr/include/hybris/camera
INCLUDEPATH += /home/jhodapp/google/ics/media_wrapper/compat/camera
INCLUDEPATH += /home/jhodapp/google/ics/media_wrapper/compat/media
LIBS += -L/usr/lib/arm-linux-gnueabi -L/home/jhodapp/google/ics/media_wrapper/hybris -lcamera -lmediaplayer

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
