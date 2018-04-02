#include "penNode.h"
#include <private/qtriangulator_p.h>
#include <QPainterPath>

#include "qmath.h"

#define GRID_SIZE 32

PenNode::PenNode()
    : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0, GL_UNSIGNED_INT)
{
    m_geometry.setDrawingMode(GL_TRIANGLES);
    setGeometry(&m_geometry);

    m_material.setColor(Qt::blue);
    setMaterial(&m_material);
}

void PenNode::update(const QPainterPath& path)
{
    const QTriangleSet triangles{ qTriangulate(path) };

    // Fill vertex buffer
    m_geometry.allocate(triangles.vertices.size() / 2, triangles.indices.size());
    QSGGeometry::Point2D *vertex = m_geometry.vertexDataAsPoint2D();
    for (int i = 0; i < m_geometry.vertexCount(); ++i)
        vertex[i].set(triangles.vertices.at(2 * i), triangles.vertices.at(2 * i + 1));

    // Fill index buffer
    uint *indices = m_geometry.indexDataAsUInt();
    if (triangles.indices.type() != QVertexIndexVector::UnsignedInt)
        qFatal("Unexpected geometry index type");
    memcpy(indices, triangles.indices.data(), triangles.indices.size() * sizeof(*indices));

    markDirty(QSGNode::DirtyGeometry);
}
