import QtQuick 2.5
import QmlVideo 1.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    visible: true;
    width: 1024;
    height: 600;

    Rectangle {
        width: parent.width / 2;
        height: parent.height / 2;
        anchors.top: parent.top;
        anchors.left: parent.left;
        border.width: 1; border.color: "#333";

        Video {
            id: video
            anchors.margins: 10;
            anchors.fill: parent;

            // 播放状态state: video.state ,
            // 可以和其枚举值进行比较: video.state == Video.Stopped

            // 播放状态的枚举值，访问形式： ${CLASS_NAME}.${ENUM_VALUE}
            // Video.Stopped
            // Video.Playing
            // Video.Paused

            TextField {
                id: videoUrl1
                anchors.bottom: start1.top
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: 400
                placeholderText: qsTr("Enter path")
                text: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4"
            }

            Button{
                id: start1
                text:"开始播放";
                width: 60
                height: 25
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 10
                onClicked: {
                    parent.stop();
                    // 获取播放状态
                    console.log("state = " + parent.state + " " + Video.Stopped)

                    //            fileName: "D:\\Test\\video\\wildlife.wmv";
                    //            fileName: "D:\\Test\\video\\testfile.mp4";
                    //            fileName: "D:\\Test\\video\\akiyo_qcif.yuv";
                    //            fileName: "D:\\Test\\video\\video.mp4";
                    //            fileName: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4";
                    // 使用URL直接播放
                    parent.play(videoUrl1.text);

                    // pause的text设置
                    pause1.text = "暂停"
                }
            }

            Button{
                id: pause1
                text:"暂停";
                width: 40
                height: 25
                anchors.bottom: start1.bottom
                anchors.left: start1.right
                anchors.leftMargin: 10
                onClicked: {
                    console.log("state = " + parent.state + " " + Video.Stopped)
                    if( parent.state === Video.Playing) {
                        this.text = "继续";
                    } else {
                        this.text = "暂停";
                    }

                    parent.playPause();
                }
            }

            Button{
                id: stop1
                text:"关闭";
                width: 40
                height: 25
                anchors.bottom: pause1.bottom
                anchors.left: pause1.right
                anchors.leftMargin: 10
                onClicked: {
                    console.log("state = " + parent.state + " " + Video.Stopped)
                    parent.stop()

                    // pause的text设置
                    pause1.text = "暂停"
                }
            }
        }
    }



    Rectangle {
        width: parent.width / 2;
        height: parent.height / 2;
        anchors.top: parent.top;
        anchors.right: parent.right;
        border.width: 1; border.color: "#333";

        Video {
            anchors.margins: 10;
            anchors.fill: parent;

            TextField {
                id: videoUrl2
                anchors.bottom: start2.top
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: 400
                placeholderText: qsTr("Enter path")
                text: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4"
            }

            Button{
                id: start2
                text:"开始播放";
                width: 60
                height: 25
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 10
                onClicked: {
                    parent.stop();
                    // 获取播放状态
                    console.log("state = " + parent.state + " " + Video.Stopped)

                    // 使用URL直接播放
                    parent.play(videoUrl2.text);

                    // pause的text设置
                    pause2.text = "暂停"
                }
            }

            Button{
                id: pause2
                text:"暂停";
                width: 40
                height: 25
                anchors.bottom: start2.bottom
                anchors.left: start2.right
                anchors.leftMargin: 10
                onClicked: {
                    console.log("state = " + parent.state + " " + Video.Stopped)
                    if( parent.state === Video.Playing) {
                        this.text = "继续";
                    } else {
                        this.text = "暂停";
                    }

                    parent.playPause();
                }
            }

            Button{
                text:"关闭";
                width: 40
                height: 25
                anchors.bottom: pause2.bottom
                anchors.left: pause2.right
                anchors.leftMargin: 10
                onClicked: {
                    console.log("state = " + parent.state + " " + Video.Stopped)
                    parent.stop()

                    // pause的text设置
                    pause2.text = "暂停"
                }
            }
        }
    }


    Rectangle {
        width: parent.width / 2;
        height: parent.height / 2;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        border.width: 1; border.color: "#333";

        Video {
            anchors.margins: 10;
            anchors.fill: parent;

            TextField {
                id: videoUrl3
                anchors.bottom: start3.top
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: 400
                placeholderText: qsTr("Enter path")
                text: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4"
            }

            Button{
                id: start3
                text:"开始播放";
                width: 60
                height: 25
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 10
                onClicked: {
                    parent.stop();
                    // 获取播放状态
                    console.log("state = " + parent.state + " " + Video.Stopped)

                    var fileName = "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4";
                    // 使用URL直接播放
                    parent.play(videoUrl3.text);

                    // pause的text设置
                    pause3.text = "暂停"
                }
            }

            Button{
                id: pause3
                text:"暂停";
                width: 40
                height: 25
                anchors.bottom: start3.bottom
                anchors.left: start3.right
                anchors.leftMargin: 10
                onClicked: {
                    console.log("state = " + parent.state + " " + Video.Stopped)
                    if( parent.state === Video.Playing) {
                        this.text = "继续";
                    } else {
                        this.text = "暂停";
                    }

                    parent.playPause();
                }
            }

            Button{
                text:"关闭";
                width: 40
                height: 25
                anchors.bottom: pause3.bottom
                anchors.left: pause3.right
                anchors.leftMargin: 10
                onClicked: {
                    console.log("state = " + parent.state + " " + Video.Stopped)
                    parent.stop()

                    // pause的text设置
                    pause3.text = "暂停"
                }
            }
        }
    }


    Rectangle {
        width: parent.width / 2;
        height: parent.height / 2;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        border.width: 1; border.color: "#333";

        Video {
            anchors.margins: 10;
            anchors.fill: parent;

            TextField {
                id: videoUrl4
                anchors.bottom: start4.top
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: 400
                placeholderText: qsTr("Enter path")
                text: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4"
            }

            Button{
                id: start4
                text:"开始播放";
                width: 60
                height: 25
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 10
                onClicked: {
                    parent.stop();
                    // 获取播放状态
                    console.log("state = " + parent.state + " " + Video.Stopped)

                    var fileName = "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4";
                    // 使用URL直接播放
                    parent.play(videoUrl4.text);

                    // pause的text设置
                    pause4.text = "暂停"
                }
            }

            Button{
                id: pause4
                text:"暂停";
                width: 40
                height: 25
                anchors.bottom: start4.bottom
                anchors.left: start4.right
                anchors.leftMargin: 10
                onClicked: {
                    console.log("state = " + parent.state + " " + Video.Stopped)
                    if( parent.state === Video.Playing) {
                        this.text = "继续";
                    } else {
                        this.text = "暂停";
                    }

                    parent.playPause();
                }
            }

            Button{
                text:"关闭";
                width: 40
                height: 25
                anchors.bottom: pause4.bottom
                anchors.left: pause4.right
                anchors.leftMargin: 10
                onClicked: {
                    console.log("state = " + parent.state + " " + Video.Stopped)
                    parent.stop()

                    // pause的text设置
                    pause4.text = "暂停"
                }
            }
        }
    }



    Rectangle {
        width: 220;
        height: 220;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        color:"transparent";

        AnimatedImage {
            id: animated;
            anchors.fill: parent;
            source: "1.gif"
        }

    }

}
