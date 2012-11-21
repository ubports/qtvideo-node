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

#include "shadervideomaterial.h"
#include "shadervideoshader.h"

#include <camera_compatibility_layer.h>
#include <media_compatibility_layer.h>

ShaderVideoMaterial::ShaderVideoMaterial(const QVideoSurfaceFormat &format)
    : m_format(format),
    m_camControl(0),
    m_mediaPlayerControl(NULL),
    m_videoShader(0)
{
}

QSGMaterialShader *ShaderVideoMaterial::createShader() const
{
    m_videoShader = new ShaderVideoShader(m_format.pixelFormat());
    return m_videoShader;
}

QSGMaterialType *ShaderVideoMaterial::type() const
{
    static QSGMaterialType theType;
    return &theType;
}

void ShaderVideoMaterial::setCamControl(CameraControl *cc)
{
    m_camControl = cc;
}

CameraControl *ShaderVideoMaterial::cameraControl() const
{
    return m_camControl;
}

void ShaderVideoMaterial::setMediaPlayerControl(MediaPlayerWrapper *mp)
{
    m_mediaPlayerControl = mp;
}

MediaPlayerWrapper *ShaderVideoMaterial::mediaplayerControl() const
{
    return m_mediaPlayerControl;
}

void ShaderVideoMaterial::bind()
{
    if (!m_camControl && !m_mediaPlayerControl) {
        qWarning() << "No valid CameraControl or MediaPlayerWrapper instance.";
        return;
    }

    if (m_camControl != NULL)
    {
        android_camera_update_preview_texture(m_camControl);
        android_camera_get_preview_texture_transformation(m_camControl, m_textureMatrix);
    }
    else if (m_mediaPlayerControl != NULL)
    {
        android_media_update_surface_texture(m_mediaPlayerControl);
        android_media_surface_texture_get_transformation_matrix(m_mediaPlayerControl, m_textureMatrix);
    }
    flipMatrixY();
    glUniformMatrix4fv(m_videoShader->m_tex_matrix, 1, GL_FALSE, m_textureMatrix);

    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

// As long as coordinates are 0..1, the y is flipped
void ShaderVideoMaterial::flipMatrixY()
{
    // reduced way to multiply the matrix with following matrix and assign it back again
    // 1  0  0  0
    // 0 -1  0  1
    // 0  0  1  0
    // 0  0  0  1
    m_textureMatrix[1] = -m_textureMatrix[1] + m_textureMatrix[3];
    m_textureMatrix[5] = -m_textureMatrix[5] + m_textureMatrix[7];
    m_textureMatrix[9] = -m_textureMatrix[9] + m_textureMatrix[11];
    m_textureMatrix[13] = -m_textureMatrix[13] + m_textureMatrix[15];
}
