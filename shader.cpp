#include <QtQuick/QSGTexture>

#include "shader.h"


BrushShader::BrushShader(): 
m_id_color(-1),
m_id_texture(-1),
m_id_textureSize(-1)
{
    setShaderSourceFile(QOpenGLShader::Vertex, ":/shaders/brush.vsh");
    setShaderSourceFile(QOpenGLShader::Fragment, ":/shaders/brush.fsh");
}

QList<QByteArray> BrushShader::attributes() const
{
    return QList<QByteArray>() << "aVertex" << "aTexCoord";
}

void BrushShader::updateState(const BrushMaterial* m, const BrushMaterial*)
{
    // Set the color
    program()->setUniformValue(m_id_color, m->color);

    // Bind the texture and set program to use texture unit 0 (the default)
    m->texture->bind();

    // Then set the texture size so we can adjust the texture coordinates accordingly in the
    // vertex shader..
    QSize s = m->texture->textureSize();
    program()->setUniformValue(m_id_textureSize, QSizeF(1.0 / s.width(), 1.0 / s.height()));
}

void BrushShader::resolveUniforms()
{
    m_id_texture = program()->uniformLocation("texture");
    m_id_textureSize = program()->uniformLocation("textureSize");
    m_id_color = program()->uniformLocation("color");

    // We will only use texture unit 0, so set it only once.
    program()->setUniformValue(m_id_texture, 0);
}
