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

#ifndef SHADERVIDEOMATERIAL_H
#define SHADERVIDEOMATERIAL_H

#include <qglobal.h>
#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
#include <QSGMaterial>
#else
#include <QtQuick/QSGMaterial>
#endif
#include <QVideoSurfaceFormat>
#include <qgl.h>

struct CameraControl;
struct MediaPlayerWrapper;
class ShaderVideoShader;

class ShaderVideoMaterial : public QSGMaterial
{
public:
    typedef void* SurfaceTextureClientHybris;
    typedef void* GLConsumerWrapperHybris;

    ShaderVideoMaterial(const QVideoSurfaceFormat &format);

    QSGMaterialShader *createShader() const;

    virtual QSGMaterialType *type() const;

    void setCamControl(CameraControl *cc);
    CameraControl *cameraControl() const;

    void setTextureId(GLuint textureId);
    GLuint textureId() const { return m_textureId; }

    void setSurfaceTextureClient(SurfaceTextureClientHybris surface_texture_client);
    void setGLConsumer(GLConsumerWrapperHybris gl_consumer);

    bool updateTexture();

    GLfloat m_textureMatrix[16];

private:
    void undoAndroidYFlip(GLfloat matrix[]);
    void printGLMaxtrix(GLfloat matrix[]);

    QVideoSurfaceFormat m_format;
    CameraControl *m_camControl;
    GLuint m_textureId;
    SurfaceTextureClientHybris m_surfaceTextureClient;
    GLConsumerWrapperHybris m_glConsumer;
    bool m_readyToRender;
    static ShaderVideoShader *m_videoShader; // the shader is cached in the Qt scene graph
};

#endif // SHADERVIDEOMATERIAL_H
