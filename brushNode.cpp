#include <QtQuick/QSGSimpleMaterialShader>
#include <QtQuick/QSGTexture>
#include <QtQuick/QQuickWindow>

#include <private/qtriangulator_p.h>
#include <QPainterPath>

#include "brushNode.h"

#define TEXTURE_SIZE 64

struct BrushMaterial
{
    ~BrushMaterial() {
        delete texture;
    }

    QColor          color;
    QSGTexture*     texture;
};

class BrushShader : public QSGSimpleMaterialShader<BrushMaterial>
{
    QSG_DECLARE_SIMPLE_SHADER(BrushShader, BrushMaterial)

public:
    BrushShader() : id_color(-1), id_texture(-1), id_textureSize(-1) {
        setShaderSourceFile(QOpenGLShader::Vertex, ":/shaders/brush.vsh");
        setShaderSourceFile(QOpenGLShader::Fragment, ":/shaders/brush.fsh");
    }

    QList<QByteArray> attributes() const
    {
        return QList<QByteArray>() << "aVertex" << "aTexCoord";
    }

    void updateState(const BrushMaterial *m, const BrushMaterial *)
    {
        // Set the color
        program()->setUniformValue(id_color, m->color);

        // Bind the texture and set program to use texture unit 0 (the default)
        m->texture->bind();

        // Then set the texture size so we can adjust the texture coordinates accordingly in the
        // vertex shader..
        QSize s = m->texture->textureSize();
        program()->setUniformValue(id_textureSize, QSizeF(1.0 / s.width(), 1.0 / s.height()));
    }

    void resolveUniforms()
    {
        id_texture = program()->uniformLocation("texture");
        id_textureSize = program()->uniformLocation("textureSize");
        id_color = program()->uniformLocation("color");

        // We will only use texture unit 0, so set it only once.
        program()->setUniformValue(id_texture, 0);
    }

private:
    int id_color;
    int id_texture;
    int id_textureSize;
};

BrushNode::BrushNode(QSGTexture* texture)
    : m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0, 0, GL_UNSIGNED_INT)
{
    setGeometry(&m_geometry);
    m_geometry.setDrawingMode(GL_TRIANGLES);

    QSGSimpleMaterial<BrushMaterial>* m = BrushShader::createMaterial();

    m->state()->texture = texture;
    m->state()->color = QColor("#000014");
    m->setFlag(QSGMaterial::Blending);

    setMaterial(m);
    setFlag(OwnsMaterial, true);
}

void BrushNode::update(const QPainterPath& path,const QRectF &bounds)
{
    const QTriangleSet triangles{ qTriangulate(path) };

    // Fill vertex buffer
    m_geometry.allocate(triangles.vertices.size() / 2, triangles.indices.size());
    QSGGeometry::TexturedPoint2D *vertex = m_geometry.vertexDataAsTexturedPoint2D();
    for (int i = 0; i < m_geometry.vertexCount(); ++i)
        vertex[i].set(triangles.vertices.at(2 * i), triangles.vertices.at(2 * i + 1), 1, 0);

    // Fill index buffer
    uint *indices = m_geometry.indexDataAsUInt();
    if (triangles.indices.type() != QVertexIndexVector::UnsignedInt)
        qFatal("Unexpected geometry index type");
    memcpy(indices, triangles.indices.data(), triangles.indices.size() * sizeof(*indices));

    markDirty(QSGNode::DirtyGeometry);
}
