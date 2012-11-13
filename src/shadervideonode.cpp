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
    /* The AAL+ CameraControl pointer is updated with every frame
     * Reason is, that the pointer might change. For example when switching the camera.
     * As the camera-backend does not know anything about this video node and vice versa,
     * it's impossible request that check/set when a new CameraControl becomes active.
     *
     * But the overhead of setting the pointer is very minor.
     */
    if (!frame.availableMetaData().contains("CamControl")) {
        qDebug() << "No camera control included in video frame";
        return;
    }

    int ci = frame.metaData("CamControl").toInt();
    m_material->setCamControl((CameraControl*)ci);

    if (ci > 0)
        markDirty(DirtyMaterial);
}
