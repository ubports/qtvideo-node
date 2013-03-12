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

#ifndef SHADERVIDEONODE_H
#define SHADERVIDEONODE_H

#include <qgl.h>
#include <QObject>
#include <private/qsgvideonode_p.h>

class CameraControl;
class ShaderVideoMaterial;
class SnapshotGenerator;

class ShaderVideoNode : public QObject, public QSGVideoNode
{
    Q_OBJECT
public:
    ShaderVideoNode(const QVideoSurfaceFormat &format);
    ~ShaderVideoNode();

    QVideoFrame::PixelFormat pixelFormat() const;
    void setCurrentFrame(const QVideoFrame &frame);

private Q_SLOTS:
    void onSetSnapshotSize(const QSize &size);
    void onTakeSnapshot(const CameraControl *control);

private:
    void getGLTextureID();

    QVideoSurfaceFormat m_format;
    ShaderVideoMaterial *m_material;
    GLuint m_textureId;
    SnapshotGenerator *m_snapshotGenerator;
};

#endif // SHADERVIDEONODE_H
