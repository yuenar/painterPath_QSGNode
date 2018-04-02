#pragma once

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

class QPainterPath;

class PenNode : public QSGGeometryNode
{
    QSGFlatColorMaterial    m_material;
    QSGGeometry             m_geometry;


public:
    PenNode();

    void update(const QPainterPath& path);
};
