CONFIG += testcase
CONFIG += no_private_qt_headers_warning c++11
TARGET = tst_shadervideonode

# We don't want to install unit tests anywhere as it doesn't make sense
target.CONFIG += no_default_install

include(../../coverage.pri)

QT += testlib multimedia opengl quick

# This is to avoid a segfault in shadervideonode.cpp when it tries to call
# glGenTextures(), since the platform currently does not support real OpenGL
# when running unit tests.
DEFINES += TST_NO_OPENGL

SOURCES += tst_shadervideonode.cpp \
    ../../src/shadervideomaterial.cpp \
    ../../src/shadervideonode.cpp \
    ../../src/shadervideoshader.cpp \
    ../../src/snapshotgenerator.cpp \
    ../../src/qsgvideonode_p.cpp \
    camera.cpp \
    player.cpp \
    video_sink.cpp

HEADERS += .  ../../src/shadervideomaterial.h \
    ../../src/shadervideonode.h \
    ../../src/shadervideoshader.h \
    ../../src/snapshotgenerator.h \
    ../../src/private/qtmultimediaquickdefs_p.h \
    ../../src/private/qsgvideonode_p.h \
    camera_compatibility_layer.h \
    surface_texture_client_hybris.h \
    video_sink.h

INCLUDEPATH += ../../src /usr/include/hybris/camera /usr/include/hybris/media /usr/include/libqtubuntu-media-signals
LIBS += -lqtubuntu-media-signals
