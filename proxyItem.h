#pragma once

#include <QtQuick/QQuickItem>
#include <QPainterPath>

class QSGGeometry;
class QSGTexture;

class ProxyItem : public QQuickItem
{
    Q_OBJECT

    QPainterPath            m_painterPath;
    QPainterPathStroker     m_stroker;
    QQuickWindow*           m_window { nullptr };

    QSGTexture* createTexture();

public:
    ProxyItem(QQuickItem *parent = 0);
    ~ProxyItem();

    QPainterPath& painterPath();
    QPainterPathStroker& stroker();

    QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *) Q_DECL_FINAL;
};

