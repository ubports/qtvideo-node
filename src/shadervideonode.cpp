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
#include <QDebug>
#include <qtubuntu_media_signals.h>

ShaderVideoNode::ShaderVideoNode(const QVideoSurfaceFormat &format) :
    m_format(format),
    m_textureId(0)
{
    m_material = new ShaderVideoMaterial(format);
    setMaterial(m_material);

    getGLTextureID();

    m_snapshotGenerator = new SnapshotGenerator;
    connect(SharedSignal::instance(), SIGNAL(setSnapshotSize(const QSize&)), this, SLOT(onSetSnapshotSize(const QSize&)));
    connect(SharedSignal::instance(), SIGNAL(takeSnapshot(const CameraControl*)), this, SLOT(onTakeSnapshot(const CameraControl*)));
}

ShaderVideoNode::~ShaderVideoNode()
{
#ifndef TST_NO_OPENGL
    glDeleteTextures(1, &m_textureId);
#endif
    m_textureId = 0;

    delete m_snapshotGenerator;
}

QVideoFrame::PixelFormat ShaderVideoNode::pixelFormat() const
{
    return m_format.pixelFormat();
}

void ShaderVideoNode::setCurrentFrame(const QVideoFrame &frame)
{
    if (!m_material->cameraControl() && !m_material->mediaplayerControl()) {
        if (!frame.availableMetaData().contains("CamControl") &&
                !frame.availableMetaData().contains("MediaPlayerControl")) {
            qWarning() << "No camera control or media player control instance included in video frame";
            return;
        }

        void *ci = 0;
        if (frame.availableMetaData().contains("CamControl")) {
            ci = frame.metaData("CamControl").value<void *>();
            if (ci == 0) {
                qWarning() << "No valid camera control pointer in video frame";
                return;
            }
            m_material->setCamControl((CameraControl*)ci);
        } else if (frame.availableMetaData().contains("MediaPlayerControl")) {
            ci = frame.metaData("MediaPlayerControl").value<void *>();
            if (ci == 0) {
                qWarning() << "No valid media player control pointer in video frame";
                return;
            }
           m_material->setMediaPlayerControl((MediaPlayerWrapper*)ci);
        }
    }

    markDirty(DirtyMaterial);
}

void ShaderVideoNode::onSetSnapshotSize(const QSize &size)
{
    Q_ASSERT(m_snapshotGenerator != NULL);
    m_snapshotGenerator->setSize(size.width(), size.height());
}

void ShaderVideoNode::onTakeSnapshot(const CameraControl *control)
{
    Q_ASSERT(m_textureId > 0);
    Q_ASSERT(control != NULL);
    Q_ASSERT(m_snapshotGenerator != NULL);
    QImage snapshot = m_snapshotGenerator->snapshot(m_textureId, control);

    // Signal the QVideoRendererControl instance that a snapshot has been taken
    Q_EMIT SharedSignal::instance()->snapshotTaken(snapshot);
}

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
