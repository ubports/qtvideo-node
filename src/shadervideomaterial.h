/*
 * Copyright (C) 2013-2014 Canonical, Ltd.
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
#include <qgl.h>
#include <QMatrix4x4>
#include <QObject>
#include <QVideoSurfaceFormat>
#include <qtubuntu_media_signals.h>

#include <memory>

namespace core { namespace ubuntu { namespace media { namespace video {
class Sink;
} } } }

struct CameraControl;
struct MediaPlayerWrapper;
class ShaderVideoShader;

class ShaderVideoMaterial : public QObject, public QSGMaterial
{
    Q_OBJECT
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
    void setGLVideoSink(const std::shared_ptr<core::ubuntu::media::video::Sink>& sink);
    const std::shared_ptr<core::ubuntu::media::video::Sink>& glVideoSink() const;

    bool updateTexture();

    GLfloat m_textureMatrix[16] {};

private Q_SLOTS:
    void onSetOrientation(const SharedSignal::Orientation& orientation, const QSize &size);

private:
    QMatrix4x4 rotateAndFlip(GLfloat *m, const SharedSignal::Orientation &orientation);
    void undoAndroidYFlip(GLfloat matrix[]);
    void printGLMaxtrix(GLfloat matrix[]);
    void printMaxtrix(float matrix[]);

    QVideoSurfaceFormat m_format;
    CameraControl *m_camControl;
    GLuint m_textureId;
    SurfaceTextureClientHybris m_surfaceTextureClient;
    std::shared_ptr<core::ubuntu::media::video::Sink> m_videoSink;
    bool m_readyToRender;
    static ShaderVideoShader *m_videoShader; // the shader is cached in the Qt scene graph
    SharedSignal::Orientation m_orientation;
    QSize m_frameSize;
};

#endif // SHADERVIDEOMATERIAL_H
