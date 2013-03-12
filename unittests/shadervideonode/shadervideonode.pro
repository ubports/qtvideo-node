CONFIG += testcase
CONFIG += no_private_qt_headers_warning
TARGET = tst_shadervideonode

include(../../coverage.pri)

QT += testlib multimedia-private qtmultimediaquicktools-private opengl

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
