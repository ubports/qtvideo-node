CONFIG += testcase
TARGET = tst_shadervideonode

include(../../coverage.pri)

QT += testlib multimedia-private qtmultimediaquicktools-private opengl

SOURCES += tst_shadervideonode.cpp \
    ../../src/shadervideomaterial.cpp \
    ../../src/shadervideonode.cpp \
    ../../src/shadervideoshader.cpp \
    camera.cpp

HEADERS += camera_compatibility_layer.h

INCLUDEPATH += ../../src
