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
//        width: 854
//        height: 480
        x: 0
        y: 0
//        border.width: 5; border.color: "blue";

        Video {
            id: video
            anchors.margins: 10;
            anchors.fill: parent;
//            fileName: "D:\\Test\\video\\wildlife.wmv";
//            fileName: "D:\\Test\\video\\testfile.mp4";
//            fileName: "D:\\Test\\video\\akiyo_qcif.yuv";
//            fileName: "D:\\Test\\video\\video.mp4";
            fileName: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4";
            Component.onCompleted: {
            }
        }

        MouseArea {
            anchors.fill: parent;
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if(mouse.button == Qt.LeftButton)
                {
                    console.log("LeftButton");
                    video.playPause();
                } else {
                    console.log("RightButton");
                    video.pause();
                    video.stop();
                }
            }
        }
    }

    Rectangle {
        width: 220;
        height: 220;
        anchors.top: parent.top;
        anchors.right: parent.right;
        color:"transparent";

//        AnimatedImage {
//            id: animated;
//            anchors.fill: parent;
//            source: "1.gif"
////            onCurrentFrameChanged: {
////                //                info.text = "%1/%2".arg(animated.currentFrame).arg(animated.frameCount);
////            }
//        }

    }



}
