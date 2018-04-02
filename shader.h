#pragma once 

#include <QColor>
#include <QSGMaterial>
#include <QSGSimpleMaterial>

struct BrushMaterial
{
    ~BrushMaterial() {
        delete texture;
    }

    QColor color;
    QSGTexture *texture;
};

class BrushShader : public QSGSimpleMaterialShader<BrushMaterial>
{
    QSG_DECLARE_SIMPLE_SHADER(BrushShader, BrushMaterial)

public:
    BrushShader();

    QList<QByteArray> attributes() const;

    void updateState(const BrushMaterial* m, const BrushMaterial*);
    void resolveUniforms();

private:
    int m_id_color;
    int m_id_texture;
    int m_id_textureSize;
};
