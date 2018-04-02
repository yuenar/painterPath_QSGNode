#include "SGPainterPathProxy.h"

#include <QtQuick/QSGNode>
#include <QtQuick/QQuickWindow>
#include <QSGTexture> 
#include <QtQuick/QSGFlatColorMaterial>

#include <private/qtriangulator_p.h>

#include "shader.h"

#define TEXTURE_SIZE 64


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

void SGPainterPathProxy::setQQuickWindow(QQuickWindow* _window)
{
    m_window = _window;
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
    QSGGeometryNode *penNode = 0;
    QSGGeometry *penGeometry = 0;

    QSGGeometryNode *brushNode = 0;
    QSGGeometry *brushGeometry = 0;

    if (!oldNode) {
        //pen node
        penNode = new QSGGeometryNode;
        penGeometry = new QSGGeometry{QSGGeometry::defaultAttributes_Point2D(), 0, 0, GL_UNSIGNED_INT};
        penGeometry->setDrawingMode(QSGGeometry::DrawTriangles);
        penGeometry->setLineWidth(5);
        penNode->setGeometry(penGeometry);

        penNode->setGeometry(penGeometry);
        penNode->setFlag(QSGNode::OwnsGeometry);
        penNode->setFlag(QSGNode::OwnsMaterial);

        //flat material
        QSGFlatColorMaterial* flatMaterial = new QSGFlatColorMaterial;
        flatMaterial->setColor(Qt::blue);
        penNode->setMaterial(flatMaterial);
        
        //brush node
        brushNode = new QSGGeometryNode;
        brushGeometry = new QSGGeometry{QSGGeometry::defaultAttributes_TexturedPoint2D(), 4};

        QSGGeometry::updateTexturedRectGeometry(brushGeometry, QRect(), QRect());
        brushGeometry->setLineWidth(3);

        brushGeometry->setDrawingMode(QSGGeometry::DrawPoints);

        brushNode->setGeometry(brushGeometry);
        brushNode->setFlag(QSGNode::OwnsGeometry);
        brushNode->setFlag(QSGNode::OwnsMaterial);

        //shader material
        // Make some noise...
        QImage image(TEXTURE_SIZE, TEXTURE_SIZE, QImage::Format_RGB32);
        uint *data = (uint *)image.bits();
        for (int i = 0; i<TEXTURE_SIZE * TEXTURE_SIZE; ++i) {
            uint g = rand() & 0xff;
            data[i] = 0xff000000 | (g << 16) | (g << 8) | g;
        }

        if (m_window) {
            QSGTexture *t = m_window->createTextureFromImage(image);
            t->setFiltering(QSGTexture::Nearest);
            t->setHorizontalWrapMode(QSGTexture::Repeat);
            t->setVerticalWrapMode(QSGTexture::Repeat);

            QSGSimpleMaterial<BrushMaterial> *m = BrushShader::createMaterial();
            m->state()->texture = t;
            m->state()->color = QColor("#ff00ff");
            m->setFlag(QSGMaterial::Blending);

            brushNode->setMaterial(m);
        }

        //appendChildNode
        penNode->appendChildNode(brushNode);
    } else {
        penNode = static_cast<QSGGeometryNode *>(oldNode);
        penGeometry = penNode->geometry();
    }

    generateTriangles(penGeometry);


    generateTriangles(brushGeometry);

    penNode->markDirty(QSGNode::DirtyGeometry);

    return penNode;
}

