import QtQuick 2.5
import QmlVideo 1.0
import QtQuick.Window 2.2

Window {
    visible: true;
    width: 1024;
    height: 600;

    Rectangle {
        width: 360
        height: 360
        x: 0
        y: 0
        border.width: 5; border.color: "blue";

        Video {
            id: video
            anchors.margins: 10;
            anchors.fill: parent;
            Component.onCompleted: {
            }
        }

    }

}
