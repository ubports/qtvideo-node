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

#ifdef QT_OPENGL_ES_2
# define TEXTURE_TARGET GL_TEXTURE_EXTERNAL_OES
#else
# define TEXTURE_TARGET GL_TEXTURE_2D
# define GL_GLEXT_PROTOTYPES
#endif

#include <QGLShaderProgram>

#include "shadervideomaterial.h"
#include "shadervideoshader.h"

#include <camera_compatibility_layer.h>
#include <media_compatibility_layer.h>

ShaderVideoShader *ShaderVideoMaterial::m_videoShader = 0;

ShaderVideoMaterial::ShaderVideoMaterial(const QVideoSurfaceFormat &format)
    : m_format(format),
    m_camControl(0),
    m_mediaPlayerControl(NULL)
{
}

QSGMaterialShader *ShaderVideoMaterial::createShader() const
{
    if (!m_videoShader) {
        m_videoShader = new ShaderVideoShader(m_format.pixelFormat());
    }
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

    if (m_camControl != NULL) {
        android_camera_update_preview_texture(m_camControl);
        android_camera_get_preview_texture_transformation(m_camControl, m_textureMatrix);
    }
    else if (m_mediaPlayerControl != NULL) {
        android_media_update_surface_texture(m_mediaPlayerControl);
        android_media_surface_texture_get_transformation_matrix(m_mediaPlayerControl, m_textureMatrix);
    }
    undoAndroidYFlip(m_textureMatrix);
    glUniformMatrix4fv(m_videoShader->m_tex_matrix, 1, GL_FALSE, m_textureMatrix);

    glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void ShaderVideoMaterial::undoAndroidYFlip(GLfloat matrix[])
{
    // The android matrix flips the y coordinate
    // The android matrix has it's texture coordinates not from 0..1 but in between there
    // The higher value is stored in m[13], the lower one is the higher one minus the height
    GLfloat height = -matrix[5]; // invert because of the flipping
    GLfloat offset = matrix[13] - height;
    matrix[5] = height;
    matrix[13] = offset;
}

/*!
 * \brief ShaderVideoMaterial::printGLMaxtrix
 * Prints a OpenGL matrix (GLfloat m[16]) to std out.
 * This function stays here for convenience in case some more debugging is necessary for the android
 * transformation matrix.
 * \param matrix Matrix to be printed to std out
 */
void ShaderVideoMaterial::printGLMaxtrix(GLfloat matrix[])
{
    qDebug() << matrix[0] << matrix[4] << matrix[8] << matrix[12];
    qDebug() << matrix[1] << matrix[5] << matrix[9] << matrix[13];
    qDebug() << matrix[2] << matrix[6] << matrix[10] << matrix[14];
    qDebug() << matrix[3] << matrix[7] << matrix[11] << matrix[15];
}
