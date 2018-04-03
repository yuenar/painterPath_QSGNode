#pragma once

#include <QtQuick/QQuickItem>
#include <QPainterPath>
#include <QBrush>

class QSGGeometry;
class QSGTexture;

class ProxyItem : public QQuickItem
{
    Q_OBJECT

    QPainterPath            m_painterPath;
    QPainterPathStroker     m_stroker;

    QBrush                  m_strokeBrush;
    QBrush                  m_fillBrush;

    QSGTexture* createTextureFromBrush(const QBrush& brush);

public:
    ProxyItem(QQuickItem *parent = 0);
    ~ProxyItem();

    QPainterPath& painterPath();
    QPainterPathStroker& stroker();

    QBrush& strokeBrush();
    QBrush& fillBrush();


    QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *) Q_DECL_FINAL;
};

