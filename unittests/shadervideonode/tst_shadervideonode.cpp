/*
 * Copyright (C) 2013-2014 Canonical, Ltd.
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

#include <core/media/video/sink.h>

#include <qtubuntu_media_signals.h>

#include "camera_compatibility_layer.h"
#include "surface_texture_client_hybris.h"

#define private public

#include "shadervideonode.h"

namespace
{
class tst_ShaderVideoNode : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void testCameraSetCurrentFrame();
    void testGLConsumerSetCurrentFrame();
    void onGLConsumerSet();

private:
    bool m_glConsumerSet;
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

struct NullSink : public core::ubuntu::media::video::Sink
{
    const core::Signal<void>& frame_available() const
    {
        static core::Signal<void> sig; return sig;
    }

    bool transformation_matrix(float*) const
    {
        return true;
    }

    bool swap_buffers() const
    {
        return true;
    }
};

}

void tst_ShaderVideoNode::initTestCase()
{
    m_glConsumerSet = false;
}

void tst_ShaderVideoNode::cleanupTestCase()
{
}

void tst_ShaderVideoNode::testCameraSetCurrentFrame()
{
    QVideoSurfaceFormat format;
    ShaderVideoNode node(format);

    CameraControl *cc = new CameraControl;
    QVideoFrame frame(new GLTextureBuffer(1), QSize(320, 240), QVideoFrame::Format_RGB32);

    node.setCurrentFrame(frame, 0);
    QCOMPARE(QVariant(QMetaType::VoidStar, node.m_material->cameraControl()),
             QVariant(QMetaType::VoidStar, 0));

    frame.setMetaData("CamControl", QVariant::fromValue(static_cast<void*>(cc)));
    node.setCurrentFrame(frame, 0);
    QCOMPARE(QVariant(QMetaType::VoidStar, node.m_material->cameraControl()),
             QVariant(QMetaType::VoidStar, cc));
}

void tst_ShaderVideoNode::testGLConsumerSetCurrentFrame()
{
    QVideoSurfaceFormat format;
    ShaderVideoNode node(format);

    connect(SharedSignal::instance(), SIGNAL(glConsumerSet()), this, SLOT(onGLConsumerSet()));

    QVideoFrame frame(new GLTextureBuffer(1), QSize(1920, 80), QVideoFrame::Format_RGB32);

    node.setCurrentFrame(frame, 0);

    std::shared_ptr<core::ubuntu::media::video::Sink> sink{new NullSink()};

    frame.setMetaData("GLVideoSink", QVariant::fromValue(sink));
    node.setCurrentFrame(frame, 0);
    QCOMPARE(node.m_material->glVideoSink(), sink);

    QVERIFY(m_glConsumerSet == true);
}

void tst_ShaderVideoNode::onGLConsumerSet()
{
    qDebug() << Q_FUNC_INFO;
    m_glConsumerSet = true;
}

QTEST_GUILESS_MAIN(tst_ShaderVideoNode)

#include "tst_shadervideonode.moc"
