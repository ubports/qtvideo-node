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

#ifndef SHADERVIDEOMATERIAL_H
#define SHADERVIDEOMATERIAL_H

#include <QSGMaterial>
#include <QVideoSurfaceFormat>
#include <qgl.h>

struct CameraControl;
struct MediaPlayerWrapper;
class ShaderVideoShader;

class ShaderVideoMaterial : public QSGMaterial
{
public:
    ShaderVideoMaterial(const QVideoSurfaceFormat &format);

    QSGMaterialShader *createShader() const;

    virtual QSGMaterialType *type() const;

    void setCamControl(CameraControl *cc);
    CameraControl *cameraControl() const;

    void setMediaPlayerControl(MediaPlayerWrapper *mp);
    MediaPlayerWrapper *mediaplayerControl() const;

    void bind();

    void flipMatrixY();

private:
    QVideoSurfaceFormat m_format;
    CameraControl *m_camControl;
    MediaPlayerWrapper *m_mediaPlayerControl;
    static ShaderVideoShader *m_videoShader; // the shader is cached in the Qt scene graph
    GLfloat m_textureMatrix[16];
    GLfloat m_flippedTextureMatrix[16];
};

#endif // SHADERVIDEOMATERIAL_H
