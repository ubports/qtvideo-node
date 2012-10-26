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

#include "camera_compatibility_layer.h"

#include <QDebug>
#include <QMutex>
#include <QtQuick/qsgmaterial.h>

class ShaderVideoShader : public QSGMaterialShader
{
public:
    ShaderVideoShader(QVideoFrame::PixelFormat pixelFormat)
        : QSGMaterialShader(),
          m_pixelFormat(pixelFormat)
    {
    }

    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial);

    virtual char const *const *attributeNames() const {
        static const char *names[] = {
            "qt_VertexPosition",
            "qt_VertexTexCoord",
            0
        };
        return names;
    }

protected:

    virtual const char *vertexShader() const {
        const char *shader =
            "#extension GL_OES_EGL_image_external : require     \n"
            "uniform highp mat4 qt_Matrix;                      \n"
            "attribute highp vec4 qt_VertexPosition;            \n"
            "attribute highp vec2 qt_VertexTexCoord;            \n"
            "varying highp vec2 qt_TexCoord;                    \n"
            "void main() {                                      \n"
            "    qt_TexCoord = qt_VertexTexCoord;               \n"
            "    gl_Position = qt_Matrix * qt_VertexPosition;   \n"
            "}";
        return shader;
    }

    virtual const char *fragmentShader() const {
        static const char *shader =
                "#extension GL_OES_EGL_image_external : require      \n"
                "uniform samplerExternalOES sTexture;                \n"
                "uniform lowp float opacity;                         \n"
                "varying highp vec2 qt_TexCoord;                     \n"
                "void main()                                         \n"
                "{                                                   \n"
                "  gl_FragColor = texture2D( sTexture, qt_TexCoord );\n"
                "}                                                   \n";
        return shader;
    }

    virtual void initialize() {
        m_id_matrix = program()->uniformLocation("qt_Matrix");
        m_id_Texture = program()->uniformLocation("sTexture");
        m_id_opacity = program()->uniformLocation("opacity");
    }

    int m_id_matrix;
    int m_id_Texture;
    int m_id_opacity;
    QVideoFrame::PixelFormat m_pixelFormat;
};

class ShaderVideoMaterial : public QSGMaterial
{
public:
    ShaderVideoMaterial(const QVideoSurfaceFormat &format)
        : m_format(format),
          m_camControl(0)
    {}
    ~ShaderVideoMaterial() {}

    QSGMaterialShader *createShader() const {
        return new ShaderVideoShader(m_format.pixelFormat());
    }

    virtual QSGMaterialType *type() const {
        static QSGMaterialType theType;
        return &theType;
    }

    void setCamControl(CameraControl *cc) {
        m_camControl = cc;
    }

    CameraControl *cameraControl() const { return m_camControl; }

    void bind()
    {
//        QMutexLocker lock(&m_frameMutex);
        if (!m_camControl) {
            qWarning() << "No valid CameraControl";
            return;
        }

//        if (!m_frame.availableMetaData().contains("CamControl")) {
//            qDebug() << "No camera control included in video frame";
//            return;
//        }

        android_camera_update_preview_texture(m_camControl);

        glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        android_camera_start_preview(m_camControl);
    }

private:
//    QVideoFrame m_frame;
//    QMutex m_frameMutex;
    QVideoSurfaceFormat m_format;
    CameraControl* m_camControl;
};



ShaderVideoNode::ShaderVideoNode(const QVideoSurfaceFormat &format) :
    m_format(format)
{
    m_material = new ShaderVideoMaterial(format);
    setMaterial(m_material);
}

void ShaderVideoNode::setCurrentFrame(const QVideoFrame &frame)
{
    QMutexLocker ml(&m_guard);
    if (! m_material->cameraControl()) {
        int ci = frame.metaData("CamControl").toInt();
        m_material->setCamControl((CameraControl*)ci);
    }
    markDirty(DirtyMaterial);
}



QList<QVideoFrame::PixelFormat> ShaderVideoNodePlugin::supportedPixelFormats(
                                        QAbstractVideoBuffer::HandleType handleType) const
{
    QList<QVideoFrame::PixelFormat> pixelFormats;

    if (handleType == QAbstractVideoBuffer::GLTextureHandle) {
        pixelFormats.append(QVideoFrame::Format_RGB32);
        pixelFormats.append(QVideoFrame::Format_ARGB32);
        pixelFormats.append(QVideoFrame::Format_BGR32);
        pixelFormats.append(QVideoFrame::Format_BGRA32);
    }

    return pixelFormats;
}

QSGVideoNode *ShaderVideoNodePlugin::createNode(const QVideoSurfaceFormat &format)
{
    if (supportedPixelFormats(format.handleType()).contains(format.pixelFormat()))
        return new ShaderVideoNode(format);

    return 0;
}



void ShaderVideoShader::updateState(const RenderState &state,
                                                QSGMaterial *newMaterial,
                                                QSGMaterial *oldMaterial)
{
    Q_UNUSED(oldMaterial);
    ShaderVideoMaterial *mat = dynamic_cast<ShaderVideoMaterial *>(newMaterial);
    program()->setUniformValue(m_id_Texture, 0);

    mat->bind();

//    if (state.isOpacityDirty()) {
//        mat->m_opacity = state.opacity();
//        mat->updateBlending();
//        program()->setUniformValue(m_id_opacity, GLfloat(mat->m_opacity));
//    }

    if (state.isMatrixDirty())
        program()->setUniformValue(m_id_matrix, state.combinedMatrix());
}
