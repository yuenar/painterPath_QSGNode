#pragma once
#include <QtQuick/QQuickItem>
#include <QPainterPath>

class QSGGeometry;

class SGPainterPathProxy : public QQuickItem
{
    Q_OBJECT

    QPainterPath            m_painterPath;
    QPainterPathStroker     m_strokerPath;

private:
    void generateTriangles(QSGGeometry* geometry);

public:
    SGPainterPathProxy(QQuickItem *parent = 0);
    ~SGPainterPathProxy();

    QPainterPath& painterPath();
    QPainterPathStroker& strokerPath();


    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) Q_DECL_FINAL;
};

