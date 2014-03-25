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

#include "shadervideonode.h"
#include "shadervideomaterial.h"
#include "snapshotgenerator.h"

#include <camera_compatibility_layer.h>
#include <qtubuntu_media_signals.h>
#include <surface_texture_client_hybris.h>

/*!
 * \brief ShaderVideoNode::ShaderVideoNode
 * \param format
 */
ShaderVideoNode::ShaderVideoNode(const QVideoSurfaceFormat &format) :
    m_format(format),
    m_textureId(0),
    m_glConsumer(0)
{
    m_material = new ShaderVideoMaterial(format);
    setMaterial(m_material);

    m_snapshotGenerator = new SnapshotGenerator;
    connect(SharedSignal::instance(), SIGNAL(setSnapshotSize(const QSize&)), this, SLOT(onSetSnapshotSize(const QSize&)));
    connect(SharedSignal::instance(), SIGNAL(takeSnapshot(const CameraControl*)), this, SLOT(onTakeSnapshot(const CameraControl*)));
}

/*!
 * \brief ShaderVideoNode::~ShaderVideoNode
 */
ShaderVideoNode::~ShaderVideoNode()
{
    deleteTextureID();
    delete m_snapshotGenerator;
}

/*!
 * \brief ShaderVideoNode::pixelFormat \reimp
 * \return
 */
QVideoFrame::PixelFormat ShaderVideoNode::pixelFormat() const
{
    return m_format.pixelFormat();
}

/*!
 * \brief ShaderVideoNode::setCurrentFrame draws the new frame. If the frame's
 * texture ID (stored in the hanlde) is empty, a new texture is created and send
 * back.
 * \param frame
 */
void ShaderVideoNode::setCurrentFrame(const QVideoFrame &frame)
{
    qDebug() << __PRETTY_FUNCTION__ << " frame: " << frame;
    void *ci = 0;
    if (frame.availableMetaData().contains("CamControl")) {
        ci = frame.metaData("CamControl").value<void *>();
        if (ci == 0) {
            qWarning() << "No valid camera control pointer in video frame";
            return;
        }
        m_material->setCamControl((CameraControl*)ci);
    } else if (frame.availableMetaData().contains("GetTextureId")) {
        qDebug() << "** Getting textureId from qtvideo-node";
        if (frame.handle().toUInt() == 0) {
            // Client requests a new texture
            if (m_textureId != 0)
                deleteTextureID();
            getGLTextureID();
            qDebug() << "qtvideo-node: got texture_id: " << m_textureId;
        }
    } else if (frame.availableMetaData().contains("GLConsumer")) {
        qDebug() << "** Setting GLConsumer instance";
        m_glConsumer = reinterpret_cast<GLConsumerWrapperHybris>(
            frame.metaData("GLConsumer").value<unsigned int>());
        m_material->setGLConsumer(m_glConsumer);
        if (m_glConsumer == 0) {
            qWarning() << "No valid GLConsumerWrapperHybris instance in video frame";
            return;
        }

        // Signal AalMediaPlayerService that glConsumer has been set
        Q_EMIT SharedSignal::instance()->glConsumerSet();
        return;
    } else if (!frame.availableMetaData().contains("CamControl") &&
               !frame.availableMetaData().contains("GetTextureId") &&
               !frame.availableMetaData().contains("GLConsumer")) {
        qWarning() << "No camera control, glConsumer instance, or texture id request included in video frame";
        m_material->setCamControl(0);
        m_material->setTextureId(0);
        return;
    }

    if (frame.handle().toUInt() == 0) {
        // Client requests a new texture
        if (m_textureId != 0)
            deleteTextureID();
        getGLTextureID();
        // Prevent drawing
        m_material->setCamControl(0);
    } else {
        // Draw the frame
        markDirty(QSGNode::DirtyMaterial);
    }
}

/*!
 * \brief ShaderVideoNode::onSetSnapshotSize sets the target size for the snapshot
 * \param size
 */
void ShaderVideoNode::onSetSnapshotSize(const QSize &size)
{
    Q_ASSERT(m_snapshotGenerator != NULL);
    m_snapshotGenerator->setSize(size.width(), size.height());
}

/*!
 * \brief ShaderVideoNode::onTakeSnapshot creates an image of the current frame
 * and sends it back to the client (camera/mediaplayer)
 * \param control
 */
void ShaderVideoNode::onTakeSnapshot(const CameraControl *control)
{
    Q_ASSERT(m_textureId > 0);
    Q_ASSERT(control != NULL);
    Q_ASSERT(m_snapshotGenerator != NULL);
    QImage snapshot = m_snapshotGenerator->snapshot(m_textureId, control);

    // Signal the QVideoRendererControl instance that a snapshot has been taken
    Q_EMIT SharedSignal::instance()->snapshotTaken(snapshot);
}

/*!
 * \brief ShaderVideoNode::getGLTextureID creates a new texture, and sends it
 * back to the client (camera/mediaplayer)
 */
void ShaderVideoNode::getGLTextureID()
{
// This is to avoid a segfault in shadervideonode.cpp when it tries to call
// glGenTextures(), since the platform currently does not support real OpenGL
// when running unit tests.
#ifndef TST_NO_OPENGL
    glGenTextures(1, &m_textureId);
#else
    m_textureId = 700001;
#endif
    if (m_textureId == 0) {
        qWarning() << "Unable to get texture ID";
        return;
    }

    Q_EMIT SharedSignal::instance()->textureCreated(static_cast<unsigned int>(m_textureId));
}

/*!
 * \brief ShaderVideoNode::deleteTextureID
 */
void ShaderVideoNode::deleteTextureID()
{
#ifndef TST_NO_OPENGL
    if (m_textureId)
        glDeleteTextures(1, &m_textureId);
#endif
    m_textureId = 0;
}
