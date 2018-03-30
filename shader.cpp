#include "shader.h"

int State::compare(const State *other) const
{
    uint rgb = color.rgba();
    uint otherRgb = other->color.rgba();

    if (rgb == otherRgb) {
        return 0;
    }
    else if (rgb < otherRgb) {
        return -1;
    }
    else {
        return 1;
    }
}

Shader::Shader()
{
    setShaderSourceFile(QOpenGLShader::Vertex, ":/shaders/brush.vsh");
    setShaderSourceFile(QOpenGLShader::Fragment, ":/shaders/brush.fsh");
}

QList<QByteArray> Shader::attributes() const
{
    return QList<QByteArray>() << "aVertex" << "aTexCoord";
}

void Shader::updateState(const State *state, const State *)
{
    program()->setUniformValue(id_color, state->color);
}

void Shader::resolveUniforms()
{
    id_color = program()->uniformLocation("color");
}
