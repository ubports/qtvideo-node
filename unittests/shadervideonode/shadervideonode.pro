CONFIG += testcase
CONFIG += no_private_qt_headers_warning
TARGET = tst_shadervideonode

# We don't want to install unit tests anywhere as it doesn't make sense
target.CONFIG += no_default_install

include(../../coverage.pri)

QT += testlib multimedia-private qtmultimediaquicktools-private opengl

# This is to avoid a segfault in shadervideonode.cpp when it tries to call
# glGenTextures(), since the platform currently does not support real OpenGL
# when running unit tests.
DEFINES += TST_NO_OPENGL

SOURCES += tst_shadervideonode.cpp \
    ../../src/shadervideomaterial.cpp \
    ../../src/shadervideonode.cpp \
    ../../src/shadervideoshader.cpp \
    ../../src/snapshotgenerator.cpp \
    camera.cpp \
    player.cpp

HEADERS += camera_compatibility_layer.h \
           media_compatibility_layer.h

INCLUDEPATH += ../../src /usr/include/hybris/camera /usr/include/libqtubuntu-media-signals
LIBS += -lqtubuntu-media-signals
