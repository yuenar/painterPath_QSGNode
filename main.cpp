#include <QtGui>
#include <QtQuick>
#include <QQmlEngine>

#include "proxyItem.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ProxyItem>("ProxyItem", 1, 0, "Proxy");

    QQuickView view;
    QSurfaceFormat format = view.format();
    format.setSamples(3);
    view.setFormat(format);

    view.setSource(QUrl("qrc:///main.qml"));

    auto rootObject = view.rootObject();

    auto proxy = rootObject->findChild<ProxyItem*>("proxy");

    if (proxy) {
        QRectF rect(QPointF(0,0), QSize(100,100));

        //path
        auto& path = proxy->painterPath();
        path.moveTo(rect.topLeft());
        path.addText(50, 50, QFont("Arial", 45), u8"бебе-12345678");

        path.lineTo(rect.center());
        path.addRect(rect);
        path.addEllipse(QPointF(100,100),20,30);

        //stroker
        auto& stroker = proxy->stroker();
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::BevelJoin);
        stroker.setWidth(5.0f);
        stroker.setDashPattern(QVector<qreal>() << 5 << 2 );
    }

    view.show();

    app.exec();
}
