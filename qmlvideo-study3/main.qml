import QtQuick 2.5
import QmlVideo 1.0
import QtQuick.Window 2.2

Window {
    visible: true;
    width: 1024;
    height: 600;

    Rectangle {
        width: 600
        height: 600
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

    Rectangle {
        width: 220;
        height: 220;
        anchors.top: parent.top;
        anchors.right: parent.right;
        color:"transparent";

        AnimatedImage {
            id: animated;
            anchors.fill: parent;
            source: "1.gif"
//            onCurrentFrameChanged: {
//                //                info.text = "%1/%2".arg(animated.currentFrame).arg(animated.frameCount);
//            }
        }

    }



}
