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
#include <QtOpenGL>
#include <QMatrix4x4>
#include <QTransform>

#include <QtCore/qdebug.h>

#include "shadervideomaterial.h"
#include "shadervideoshader.h"

#include <camera_compatibility_layer.h>
#include <surface_texture_client_hybris.h>

ShaderVideoShader *ShaderVideoMaterial::m_videoShader = 0;

ShaderVideoMaterial::ShaderVideoMaterial(const QVideoSurfaceFormat &format)
    : m_format(format),
      m_camControl(0),
      m_textureId(0),
      m_surfaceTextureClient(0),
      m_glConsumer(0),
      m_readyToRender(false)
{
    // Initialize m_textureMatrix
    for (uint8_t i=0; i < 16; i++)
        m_textureMatrix[i] = 0;

#if 0
    // FIXME: Temporary hack (android * container)
    m_textureMatrix[0] = 0;
    m_textureMatrix[1] = 1;
    m_textureMatrix[2] = 0;
    m_textureMatrix[3] = 0;

    m_textureMatrix[4] = 1;
    m_textureMatrix[5] = 0;
    m_textureMatrix[6] = 0;
    m_textureMatrix[7] = 0;

    m_textureMatrix[8] = 0;
    m_textureMatrix[9] = 0;
    m_textureMatrix[10] = 1;
    m_textureMatrix[11] = 0;

    m_textureMatrix[12] = 0;
    m_textureMatrix[13] = 1;
    m_textureMatrix[14] = 0;
    m_textureMatrix[15] = 1;
#endif

#if 1
    // FIXME: Temporary hack (android * container * scaling)
    m_textureMatrix[0] = 0;
    m_textureMatrix[1] = 1;
    m_textureMatrix[2] = 0;
    m_textureMatrix[3] = 0;

    m_textureMatrix[4] = 1;
    m_textureMatrix[5] = 0;
    m_textureMatrix[6] = 0;
    m_textureMatrix[7] = 0;

    m_textureMatrix[8] = 0;
    m_textureMatrix[9] = 0;
    m_textureMatrix[10] = 1;
    m_textureMatrix[11] = 0;

    m_textureMatrix[12] = 0;
    m_textureMatrix[13] = 1;
    m_textureMatrix[14] = 0;
    m_textureMatrix[15] = 1;
#endif
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
    if (m_camControl != cc) {
        m_camControl = cc;
    }
}

CameraControl *ShaderVideoMaterial::cameraControl() const
{
    return m_camControl;
}

void ShaderVideoMaterial::setTextureId(GLuint textureId)
{
    m_textureId = textureId;
}

void ShaderVideoMaterial::setSurfaceTextureClient(SurfaceTextureClientHybris surface_texture_client)
{
    m_surfaceTextureClient = surface_texture_client;
}

void ShaderVideoMaterial::setGLConsumer(GLConsumerWrapperHybris gl_consumer)
{
    m_glConsumer = gl_consumer;
}

GLConsumerWrapperHybris ShaderVideoMaterial::glConsumer() const
{
    return m_glConsumer;
}

bool ShaderVideoMaterial::updateTexture()
{
    bool textureDirty = false;

    if (!m_camControl && !m_textureId && !m_glConsumer) {
        return false;
    }

    if (m_camControl != NULL) {
        android_camera_update_preview_texture(m_camControl);
        android_camera_get_preview_texture_transformation(m_camControl, m_textureMatrix);
        textureDirty = true;
    } else if (m_glConsumer != NULL && !m_readyToRender) {
        m_readyToRender = true;
    } else if (m_glConsumer != NULL && m_readyToRender) {
        gl_consumer_update_texture(m_glConsumer);
        //gl_consumer_get_transformation_matrix(m_glConsumer, static_cast<float*>(m_textureMatrix));
        //printGLMaxtrix(m_textureMatrix);
        textureDirty = true;
    }

    QMatrix4x4 qandcont(0, 1, 0, 0,
                        1, 0, 0, 1,
                        0, 0, 1, 0,
                        0, 0, 0, 1);

    Matrix andcont = { 0, 1, 0, 0,
                       1, 0, 0, 1,
                       0, 0, 1, 0,
                       0, 0, 0, 1 };

    Matrix scaling = { 0.5625, 0, 0, 0,
                       0, 1.7778, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1 };

    printMaxtrix(andcont.m);

    const float aspectRatio = 1280.0 / 720.0;
    qandcont.ortho(-aspectRatio, aspectRatio, -1, 1, -1, 1);
    //qandcont.scale(0.5625, 1.7778);
    qDebug() << "aspectRatio: " << aspectRatio;
    qDebug() << "qandcont: " << qandcont;
#if 0
    qDebug() << " * ";
    printMaxtrix(scaling.m);
    Matrix res;
    multiplyMatrix(&res, &andcont, &scaling);
    qDebug() << "res: ";
    printMaxtrix(res.m);

    convertToGLMatrix(m_textureMatrix, &res);
#endif


    //printGLMaxtrix(m_textureMatrix);
    //undoAndroidYFlip(m_textureMatrix);
    qDebug() << "-------------------";
    //printGLMaxtrix(m_textureMatrix);
    //glUniformMatrix4fv(m_videoShader->m_tex_matrix, 1, GL_TRUE, m_textureMatrix);
    glUniformMatrix4fv(m_videoShader->m_tex_matrix, 1, GL_TRUE, qandcont.data());

    glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(TEXTURE_TARGET, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureDirty;
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
 * Prints an EGL matrix (GLfloat m[16]) to std out.
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

void ShaderVideoMaterial::printMaxtrix(float matrix[])
{
    qDebug() << matrix[0] << matrix[1] << matrix[2] << matrix[3];
    qDebug() << matrix[4] << matrix[5] << matrix[6] << matrix[7];
    qDebug() << matrix[8] << matrix[9] << matrix[10] << matrix[11];
    qDebug() << matrix[12] << matrix[13] << matrix[14] << matrix[15];
}

void ShaderVideoMaterial::vectorSet(ShaderVideoMaterial::Vector *v, float x, float y, float z, float w)
{
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
}

void ShaderVideoMaterial::multiplyMatrix(ShaderVideoMaterial::Matrix* __restrict m, const ShaderVideoMaterial::Matrix* __restrict m1,
        const ShaderVideoMaterial::Matrix* __restrict m2)
{
    vectorSet(&m->v0,
            m1->m00*m2->m00 + m1->m01*m2->m10 + m1->m02*m2->m20 + m1->m03*m2->m30,
            m1->m00*m2->m01 + m1->m01*m2->m11 + m1->m02*m2->m21 + m1->m03*m2->m31,
            m1->m00*m2->m02 + m1->m01*m2->m12 + m1->m02*m2->m22 + m1->m03*m2->m32,
            m1->m00*m2->m03 + m1->m01*m2->m13 + m1->m02*m2->m23 + m1->m03*m2->m33);
    vectorSet(&m->v1,
            m1->m10*m2->m00 + m1->m11*m2->m10 + m1->m12*m2->m20 + m1->m13*m2->m30,
            m1->m10*m2->m01 + m1->m11*m2->m11 + m1->m12*m2->m21 + m1->m13*m2->m31,
            m1->m10*m2->m02 + m1->m11*m2->m12 + m1->m12*m2->m22 + m1->m13*m2->m32,
            m1->m10*m2->m03 + m1->m11*m2->m13 + m1->m12*m2->m23 + m1->m13*m2->m33);
    vectorSet(&m->v2,
            m1->m20*m2->m00 + m1->m21*m2->m10 + m1->m22*m2->m20 + m1->m23*m2->m30,
            m1->m20*m2->m01 + m1->m21*m2->m11 + m1->m22*m2->m21 + m1->m23*m2->m31,
            m1->m20*m2->m02 + m1->m21*m2->m12 + m1->m22*m2->m22 + m1->m23*m2->m32,
            m1->m20*m2->m03 + m1->m21*m2->m13 + m1->m22*m2->m23 + m1->m23*m2->m33);
    vectorSet(&m->v3,
            m1->m30*m2->m00 + m1->m31*m2->m10 + m1->m32*m2->m20 + m1->m33*m2->m30,
            m1->m30*m2->m01 + m1->m31*m2->m11 + m1->m32*m2->m21 + m1->m33*m2->m31,
            m1->m30*m2->m02 + m1->m31*m2->m12 + m1->m32*m2->m22 + m1->m33*m2->m32,
            m1->m30*m2->m03 + m1->m31*m2->m13 + m1->m32*m2->m23 + m1->m33*m2->m33);
}

void ShaderVideoMaterial::convertToGLMatrix(GLfloat *m, const Matrix* __restrict m1)
{
    if (m == NULL || m1 == NULL)
        return;

    m[0] = m1->m[0];
    m[1] = m1->m[4];
    m[2] = m1->m[8];
    m[3] = m1->m[12];
    m[4] = m1->m[1];
    m[5] = m1->m[5];
    m[6] = m1->m[9];
    m[7] = m1->m[13];
    m[8] = m1->m[2];
    m[9] = m1->m[6];
    m[10] = m1->m[10];
    m[11] = m1->m[14];
    m[12] = m1->m[3];
    m[13] = m1->m[7];
    m[14] = m1->m[11];
    m[15] = m1->m[15];
}

void ShaderVideoMaterial::convertToMatrix(Matrix* __restrict m, const GLfloat* m1)
{
    if (m == NULL || m1 == NULL)
        return;

    m->m[0] = m1[0];
    m->m[1] = m1[4];
    m->m[2] = m1[8];
    m->m[3] = m1[12];
    m->m[4] = m1[1];
    m->m[5] = m1[5];
    m->m[6] = m1[9];
    m->m[7] = m1[13];
    m->m[8] = m1[2];
    m->m[9] = m1[6];
    m->m[10] = m1[10];
    m->m[11] = m1[14];
    m->m[12] = m1[3];
    m->m[13] = m1[7];
    m->m[14] = m1[11];
    m->m[15] = m1[15];
}
