#include "SGPainterPathProxy.h"

#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgflatcolormaterial.h>
#include <private/qtriangulator_p.h>

#include "shader.h"


SGPainterPathProxy::SGPainterPathProxy(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

SGPainterPathProxy::~SGPainterPathProxy()
{

}

QPainterPath& SGPainterPathProxy::painterPath()
{
    return m_painterPath;
}

QPainterPathStroker& SGPainterPathProxy::strokerPath()
{
    return m_strokerPath;
}

void SGPainterPathProxy::generateTriangles(QSGGeometry* geometry)
{
    const QTriangleSet triangles{ qTriangulate(m_strokerPath.createStroke(m_painterPath)) };

    // Fill vertex buffer
    geometry->allocate(triangles.vertices.size() / 2, triangles.indices.size());
    QSGGeometry::Point2D *vertex = geometry->vertexDataAsPoint2D();
    for (int i = 0; i < geometry->vertexCount(); ++i)
        vertex[i].set(triangles.vertices.at(2 * i), triangles.vertices.at(2 * i + 1));

    // Fill index buffer
    uint *indices = geometry->indexDataAsUInt();
    if (triangles.indices.type() != QVertexIndexVector::UnsignedInt)
        qFatal("Unexpected geometry index type");
    memcpy(indices, triangles.indices.data(), triangles.indices.size() * sizeof(*indices));
}

QSGNode *SGPainterPathProxy::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *node = 0;
    QSGGeometry *geometry = 0;

    if (!oldNode) {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry{QSGGeometry::defaultAttributes_Point2D(), 0, 0, GL_UNSIGNED_INT};
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        //shader material
        QSGSimpleMaterial<State> *material = Shader::createMaterial();
        material->setFlag(QSGMaterial::Blending);

        //flat material
        QSGFlatColorMaterial* flatMaterial = new QSGFlatColorMaterial;
        flatMaterial->setColor(Qt::blue);

        //node->setMaterial(flatMaterial);
        node->setMaterial(material);

        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
    }

    generateTriangles(geometry);

    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}

