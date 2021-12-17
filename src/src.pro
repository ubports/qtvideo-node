TARGET = shadervideonode
TEMPLATE = lib
CONFIG += plugin c++11 link_pkgconfig
QT += multimedia opengl quick-private quick
PKGCONFIG += MediaHub

include(../coverage.pri)

PLUGIN_TYPE = video/videonode
PLUGIN_EXTENDS = quick
PLUGIN_CLASS_NAME = ShaderVideoNodePlugin

target.path += $$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS = target

INCLUDEPATH += /usr/include/hybris/camera /usr/include/hybris/media /usr/include/libqtubuntu-media-signals
LIBS += -lhybris_ics -lcamera -lmedia -lqtubuntu-media-signals

OTHER_FILES += shadervideonode.json

HEADERS += \
    shadervideonodeplugin.h \
    shadervideomaterial.h \
    shadervideoshader.h \
    shadervideonode.h \
    snapshotgenerator.h

SOURCES += \
    shadervideonodeplugin.cpp \
    shadervideomaterial.cpp \
    shadervideoshader.cpp \
    shadervideonode.cpp \
    snapshotgenerator.cpp \
    qsgvideonode_p.cpp
