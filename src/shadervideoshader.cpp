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

#include "shadervideoshader.h"
#include "shadervideomaterial.h"

ShaderVideoShader::ShaderVideoShader(QVideoFrame::PixelFormat pixelFormat)
    : QSGMaterialShader(),
    m_pixelFormat(pixelFormat)
{
}

void ShaderVideoShader::updateState(const RenderState &state,
                                                QSGMaterial *newMaterial,
                                                QSGMaterial *oldMaterial)
{
    Q_UNUSED(oldMaterial);
    ShaderVideoMaterial *mat = dynamic_cast<ShaderVideoMaterial *>(newMaterial);
    program()->setUniformValue(m_id_Texture, 0);

    if (state.isMatrixDirty())
        program()->setUniformValue(m_id_matrix, state.combinedMatrix());

    mat->bind();

//    if (state.isOpacityDirty()) {
//        mat->m_opacity = state.opacity();
//        mat->updateBlending();
//        program()->setUniformValue(m_id_opacity, GLfloat(mat->m_opacity));
//    }
}

char const *const *ShaderVideoShader::attributeNames() const
{
    static const char *names[] = {
        "qt_VertexPosition",
        "qt_VertexTexCoord",
        0
    };
    return names;
}

const char *ShaderVideoShader::vertexShader() const
{
    const char *shader =
        "#extension GL_OES_EGL_image_external : require     \n"
        "uniform highp mat4 qt_Matrix;                      \n"
        "attribute highp vec4 qt_VertexPosition;            \n"
        "attribute highp vec2 qt_VertexTexCoord;            \n"
        "varying highp vec2 qt_TexCoord;                    \n"
        "uniform mat4 s_tex_Matrix;                         \n"
        "void main() {                                      \n"
        "    qt_TexCoord = (s_tex_Matrix * vec4(qt_VertexTexCoord, 0.0, 1.0)).xy;\n"
        "    gl_Position = qt_Matrix * qt_VertexPosition;   \n"
        "}";
    return shader;
}

const char *ShaderVideoShader::fragmentShader() const
{
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

void ShaderVideoShader::initialize()
{
    m_id_matrix = program()->uniformLocation("qt_Matrix");
    m_id_Texture = program()->uniformLocation("sTexture");
    m_id_opacity = program()->uniformLocation("opacity");
    m_tex_matrix = program()->uniformLocation("s_tex_Matrix");
}
