TARGET = shadervideonode
QT += quick opengl multimedia-private qtmultimediaquicktools-private

PLUGIN_TYPE = video/videonode

load(qt_plugin)
target.path += $$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS = target

INCLUDEPATH += /usr/include/hybris/camera
LIBS += -L/usr/lib/arm-linux-gnueabi -lcamera

OTHER_FILES += shadervideonode.json

HEADERS += \
    shadervideonodeplugin.h

SOURCES += \
    shadervideonodeplugin.cpp
