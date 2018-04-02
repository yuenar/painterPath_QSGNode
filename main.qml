import QtQuick 2.0
import ProxyItem 1.0

Item {
    width: 400
    height: 300

    Proxy{
        objectName: "proxy"
        anchors.fill: parent
        anchors.margins: 5
    }
}
