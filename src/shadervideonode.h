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

#ifndef SHADERVIDEONODE_H
#define SHADERVIDEONODE_H

#include <private/qsgvideonode_p.h>

class ShaderVideoMaterial;

class ShaderVideoNode : public QSGVideoNode
{
public:
    ShaderVideoNode(const QVideoSurfaceFormat &format);

    QVideoFrame::PixelFormat pixelFormat() const;
    void setCurrentFrame(const QVideoFrame &frame);

private:
    QVideoSurfaceFormat m_format;
    ShaderVideoMaterial *m_material;
};

#endif // SHADERVIDEONODE_H
