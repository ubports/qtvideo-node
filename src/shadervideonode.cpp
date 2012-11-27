/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *  Guenter Schwann <guenter.schwann@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "shadervideonode.h"
#include "shadervideomaterial.h"

#include <camera_compatibility_layer.h>
#include <media_compatibility_layer.h>

ShaderVideoNode::ShaderVideoNode(const QVideoSurfaceFormat &format) :
    m_format(format)
{
    m_material = new ShaderVideoMaterial(format);
    setMaterial(m_material);
}

QVideoFrame::PixelFormat ShaderVideoNode::pixelFormat() const
{
    return m_format.pixelFormat();
}

void ShaderVideoNode::setCurrentFrame(const QVideoFrame &frame)
{
    if (!m_material->cameraControl() && !m_material->mediaplayerControl()) {
        if (!frame.availableMetaData().contains("CamControl") && !frame.availableMetaData().contains("MediaPlayerControl")) {
            qWarning() << "No camera control or media player control instance included in video frame";
            return;
        }

        int ci = 0;
        if (frame.availableMetaData().contains("CamControl")) {
            ci = frame.metaData("CamControl").toInt();
            if (ci == 0) {
                qWarning() << "No valid camera control pointer in video frame";
                return;
            }
            m_material->setCamControl((CameraControl*)ci);
        } else if (frame.availableMetaData().contains("MediaPlayerControl")) {
            ci = frame.metaData("MediaPlayerControl").toInt();
            if (ci == 0) {
                qWarning() << "No valid media player control pointer in video frame";
                return;
            }
           m_material->setMediaPlayerControl((MediaPlayerWrapper*)ci);
        }
    }

    markDirty(DirtyMaterial);
}
