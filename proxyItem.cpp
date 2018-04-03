#include "proxyItem.h"

#include <QtQuick/QQuickWindow>
#include <QSGTexture> 
#include <QPainter> 

#include "pathNode.h"

#define TEXTURE_SIZE 15

struct PainterPathNode : public QSGNode
{
    PathNode*           strokeNode {nullptr};
    PathNode*           fillNode {nullptr};
    QSGTransformNode*   transformNode {nullptr};
};

ProxyItem::ProxyItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

ProxyItem::~ProxyItem()
{

}

QPainterPath& ProxyItem::painterPath()
{
    return m_painterPath;
}

QPainterPathStroker& ProxyItem::stroker()
{
    return m_stroker;
}

QBrush& ProxyItem::strokeBrush()
{
    return m_strokeBrush;
}

QBrush& ProxyItem::fillBrush()
{
    return m_fillBrush;
}

QSGTexture* ProxyItem::createTextureFromBrush(const QBrush& _brush)
{
    QImage image(TEXTURE_SIZE, TEXTURE_SIZE, QImage::Format_RGB32);

    QPainter painter;
    painter.begin(&image);
    painter.fillRect(QRect(0,0,TEXTURE_SIZE, TEXTURE_SIZE),_brush);
    painter.end();

    auto quickwindow = window();

    if (quickwindow) {
        QSGTexture *t = quickwindow->createTextureFromImage(image);
        t->setFiltering(QSGTexture::Linear);
        t->setHorizontalWrapMode(QSGTexture::Repeat);
        t->setVerticalWrapMode(QSGTexture::Repeat);

        return t;
    }

    return nullptr;
}

QSGNode *ProxyItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    PainterPathNode *n = static_cast<PainterPathNode*>(oldNode);

    QRectF rect = boundingRect();

    if (rect.isEmpty())
    {
        delete n;
        return nullptr;
    }

    if (!n) {
        n = new PainterPathNode();

        n->strokeNode = new PathNode(createTextureFromBrush(m_strokeBrush),m_strokeBrush.color());
        n->fillNode = new PathNode(createTextureFromBrush(m_fillBrush), m_fillBrush.color());

        //отрисовываются в порядке добавления
        n->appendChildNode(n->fillNode);
        n->fillNode->setFlag(QSGNode::OwnedByParent);

        n->appendChildNode(n->strokeNode);
        n->strokeNode->setFlag(QSGNode::OwnedByParent);
    }

    n->fillNode->update(m_painterPath);
    n->strokeNode->update(m_stroker.createStroke(m_painterPath));

    return n;
}

