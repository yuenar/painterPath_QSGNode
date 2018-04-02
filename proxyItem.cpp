#include "proxyItem.h"

#include <QtQuick/QSGNode>
#include <QtQuick/QQuickWindow>
#include <QSGTexture> 
#include <QPainter> 

#include "shader.h"
#include "penNode.h"
#include "brushNode.h"

#define TEXTURE_SIZE 15

class ProxyNode : public QSGNode
{
public:
    PenNode * pen;
    BrushNode * brush;
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

QSGTexture* ProxyItem::createTexture()
{
    // Make some noise...
    QImage image(TEXTURE_SIZE, TEXTURE_SIZE, QImage::Format_RGB32);

    QPainter p;
   
    p.begin(&image);
    p.drawText(10, 10, "333");
    p.drawLine(0, 0, 15, 15);
    p.end();

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
    ProxyNode *n = static_cast<ProxyNode*>(oldNode);

    QRectF rect = boundingRect();

    if (rect.isEmpty()) {
        delete n;
        return 0;
    }

    if (!n) {
        n = new ProxyNode();

        n->pen = new PenNode();

        auto texture = createTexture();
        n->brush = new BrushNode(texture);

        n->appendChildNode(n->pen);
        n->appendChildNode(n->brush);
    }

    n->pen->update(m_stroker.createStroke(m_painterPath));


    n->brush->update(boundingRect());
    

    return n;
}

