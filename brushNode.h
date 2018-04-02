#pragma once

#include <QSGGeometryNode>

class BrushNode : public QSGGeometryNode
{
    QSGGeometry             m_geometry;

public:
    BrushNode(QSGTexture * texture);

    void update(const QRectF &bounds);
};
