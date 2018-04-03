#pragma once

#include <QSGGeometryNode>


class BrushNode : public QSGGeometryNode
{
    QSGGeometry             m_geometry;

public:
    BrushNode(QSGTexture * texture);

    void update(const QPainterPath& path,const QRectF &bounds);
};
