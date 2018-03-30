#pragma once 

#include <QColor>

#include <QSGMaterial>
#include <QSGSimpleMaterial>

/// <summary>
/// State
/// </summary>
struct State
{
    QColor color;

    int compare(const State *other) const;
};

/// <summary>
/// Shader
/// </summary>
class Shader : public QSGSimpleMaterialShader<State>
{
    QSG_DECLARE_SIMPLE_COMPARABLE_SHADER(Shader, State);

    Shader();

public:
    //const char* vertexShader() const;
    //const char* fragmentShader() const;

    QList<QByteArray> attributes() const;

    void updateState(const State *state, const State *);

    void resolveUniforms();

private:
    int id_color;
};
