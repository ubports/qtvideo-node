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

#include "shadervideonodeplugin.h"
#include "shadervideonode.h"

QList<QVideoFrame::PixelFormat> ShaderVideoNodePlugin::supportedPixelFormats(
                                        QAbstractVideoBuffer::HandleType handleType) const
{
    QList<QVideoFrame::PixelFormat> pixelFormats;

    if (handleType == QAbstractVideoBuffer::GLTextureHandle) {
        pixelFormats.append(QVideoFrame::Format_RGB32);
        pixelFormats.append(QVideoFrame::Format_ARGB32);
        pixelFormats.append(QVideoFrame::Format_BGR32);
        pixelFormats.append(QVideoFrame::Format_BGRA32);
        pixelFormats.append(QVideoFrame::Format_User); // handling a GL texture of the aal library for the camera
    }

    return pixelFormats;
}

QSGVideoNode *ShaderVideoNodePlugin::createNode(const QVideoSurfaceFormat &format)
{
    if (supportedPixelFormats(format.handleType()).contains(format.pixelFormat()))
        return new ShaderVideoNode(format);

    return 0;
}
