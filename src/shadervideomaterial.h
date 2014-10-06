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
    GLConsumerWrapperHybris glConsumer() const;

    bool updateTexture();

    GLfloat m_textureMatrix[16];

private:
    // 4-component vector
    typedef struct {
        union {
            struct { float x, y, z, w; };
            float v[4];
        };
    } Vector;

    // 4x4 matrix
    typedef struct {
        union {
            struct {
                float m00, m01, m02, m03;
                float m10, m11, m12, m13;
                float m20, m21, m22, m23;
                float m30, m31, m32, m33;
            };
            float m[16];
            struct { Vector v0, v1, v2, v3; };
            Vector v[4];
        };
    } Matrix;

    void undoAndroidYFlip(GLfloat matrix[]);
    void printGLMaxtrix(GLfloat matrix[]);
    void printMaxtrix(float matrix[]);

    // Vector initialization.
    void vectorSet(Vector *v, float x, float y, float z, float w);
    // Multiply two matrices together. The result is stored in |m|
    void multiplyMatrix(Matrix* __restrict m, const Matrix* __restrict m1,
            const Matrix* __restrict m2);
    // Converts the ordering of a local Matrix to a GLfloat matrix
    void convertToGLMatrix(GLfloat *m, const Matrix* __restrict m1);
    // Converts the ordering of a GLfloat matrix to a local Matrix
    void convertToMatrix(Matrix* __restrict m, const GLfloat* m1);

    QVideoSurfaceFormat m_format;
    CameraControl *m_camControl;
    GLuint m_textureId;
    SurfaceTextureClientHybris m_surfaceTextureClient;
    GLConsumerWrapperHybris m_glConsumer;
    bool m_readyToRender;
    static ShaderVideoShader *m_videoShader; // the shader is cached in the Qt scene graph
};

#endif // SHADERVIDEOMATERIAL_H
