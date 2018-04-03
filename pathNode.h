#pragma once

#include <QSGGeometryNode>

class PathNode : public QSGGeometryNode
{
    QSGGeometry             m_geometry;

public:
    PathNode(QSGTexture* texture, const QColor& color);

    void update(const QPainterPath& path);
};
