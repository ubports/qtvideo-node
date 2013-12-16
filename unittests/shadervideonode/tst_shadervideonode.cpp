/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtTest/QtTest>

#include <QImage>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>

#include <shadervideomaterial.h>

#include "camera_compatibility_layer.h"
#include "surface_texture_client_hybris.h"

#define private public
#include "shadervideonode.h"

class tst_ShaderVideoNode : public QObject
{
    Q_OBJECT
private slots:
    void testCameraSetCurrentFrame();
    void testTextureIdSetCurrentFrame();
};

class GLTextureBuffer : public QAbstractVideoBuffer
{
public:
    GLTextureBuffer(unsigned int textureId) :
        QAbstractVideoBuffer(QAbstractVideoBuffer::GLTextureHandle),
        m_textureId(textureId)
    {
    }

    MapMode mapMode() const { return NotMapped; }
    uchar *map(MapMode mode, int *numBytes, int *bytesPerLine)
    {
        Q_UNUSED(mode);
        Q_UNUSED(numBytes);
        Q_UNUSED(bytesPerLine);
        return (uchar*)0;
    }

    void unmap() {}

    QVariant handle() const
    {
        return QVariant::fromValue<unsigned int>(m_textureId);
    }


private:
    unsigned int m_textureId;
};

void tst_ShaderVideoNode::testCameraSetCurrentFrame()
{
    QVideoSurfaceFormat format;
    ShaderVideoNode node(format);

    CameraControl *cc = new CameraControl;
    QVideoFrame frame(new GLTextureBuffer(1), QSize(320, 240), QVideoFrame::Format_RGB32);

    node.setCurrentFrame(frame);
    QCOMPARE(QVariant(QMetaType::VoidStar, node.m_material->cameraControl()),
             QVariant(QMetaType::VoidStar, 0));

    frame.setMetaData("CamControl", QVariant::fromValue((void*)cc));
    node.setCurrentFrame(frame);
    QCOMPARE(QVariant(QMetaType::VoidStar, node.m_material->cameraControl()),
             QVariant(QMetaType::VoidStar, cc));
}

void tst_ShaderVideoNode::testTextureIdSetCurrentFrame()
{
    QVideoSurfaceFormat format;
    ShaderVideoNode node(format);

    QVideoFrame frame(new GLTextureBuffer(1), QSize(1920, 80), QVideoFrame::Format_RGB32);

    node.setCurrentFrame(frame);
    QCOMPARE(node.m_material->textureId(), (const GLuint)0);

    frame.setMetaData("TextureId", 70001);
    frame.setMetaData("SurfaceTextureClient", 0x1);
    node.setCurrentFrame(frame);
    QCOMPARE(node.m_material->textureId(), (const GLuint)70001);
}

QTEST_MAIN(tst_ShaderVideoNode)

#include "tst_shadervideonode.moc"
