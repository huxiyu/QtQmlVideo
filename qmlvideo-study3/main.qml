import QtQuick 2.5
import QmlVideo 1.0
import QtQuick.Window 2.2

Window {
    visible: true;
    width: 1280;
    height: 800;

    Rectangle {
        id: rect;
        width: 1280;
        height: 720;
        //        width: 854
        //        height: 480
        x: 0
        y: 0
        border.width: 5; border.color: "blue";
        color: "transparent"
        focus: true;

        Keys.enabled: true;
        Keys.onEscapePressed:
        {
            console.log("onEscapePressed")
            //            video.setState("Stopped");
            Qt.quit();
        }


        Rectangle {
            width: rect.width / 2;
            height: rect.height / 2;
            anchors.top: parent.top;
            anchors.left: parent.left;
            color:"white";
            border.width: 1; border.color: "Red";

            Video {
                id: video1;
                anchors.fill: parent;
                //                            fileName: "D:\\Test\\video\\wildlife.wmv";
                //            fileName: "D:\\Test\\video\\testfile.mp4";
                //            fileName: "D:\\Test\\video\\akiyo_qcif.yuv";
                //            fileName: "D:\\Test\\video\\video.mp4";
                fileName: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4";
                state: Video.Stopped;

                MouseArea {
                    anchors.fill: parent;
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        //                        video1.setState(Video.Playing)
                        if(mouse.button == Qt.LeftButton)
                        {
                            console.log("pause")
                            parent.pause();
                        }
                        else
                        {
                            console.log("play")
                            parent.stop();
                            parent.play();
                        }
                    }
                }
            }
        }


        Rectangle {
            width: rect.width / 2;
            height: rect.height / 2;
            anchors.top: parent.top;
            anchors.right: parent.right;
            color:"white";
            border.width: 1; border.color: "Red";

            Video {
                id: video2;
                anchors.fill: parent;
                //                                fileName: "D:\\Test\\video\\wildlife.wmv";
                //            fileName: "D:\\Test\\video\\testfile.mp4";
//                fileName: "D:\\Test\\video\\akiyo_qcif.yuv";
                //                fileName: "D:\\Test\\video\\video.mp4";
                fileName: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4";
                state: Video.Stopped;

                MouseArea {
                    anchors.fill: parent;
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        //                        video1.setState(Video.Playing)
                        if(mouse.button == Qt.LeftButton)
                        {
                            console.log("pause")
                            parent.pause();
                        }
                        else
                        {
                            console.log("play")
                            parent.stop();
                            parent.play();
                        }
                    }
                }
            }
        }

        Rectangle {
            width: rect.width / 2;
            height: rect.height / 2;
            anchors.bottom: parent.bottom;
            anchors.left: parent.left;
            color:"white";
            border.width: 1; border.color: "Red";

            Video {
                id: video3;
                anchors.fill: parent;
                //                                fileName: "D:\\Test\\video\\wildlife.wmv";
                //            fileName: "D:\\Test\\video\\testfile.mp4";
//                fileName: "D:\\Test\\video\\akiyo_qcif.yuv";
                //                fileName: "D:\\Test\\video\\video.mp4";
                fileName: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4";
                state: Video.Stopped;

                MouseArea {
                    anchors.fill: parent;
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        //                        video1.setState(Video.Playing)
                        if(mouse.button == Qt.LeftButton)
                        {
                            console.log("pause")
                            parent.pause();
                        }
                        else
                        {
                            console.log("play")
                            parent.stop();
                            parent.play();
                        }
                    }
                }
            }
        }

        Rectangle {
            width: rect.width / 2;
            height: rect.height / 2;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;
            color:"white";
            border.width: 1; border.color: "Red";

            Video {
                id: video4;
                anchors.fill: parent;
                //                                fileName: "D:\\Test\\video\\wildlife.wmv";
                //            fileName: "D:\\Test\\video\\testfile.mp4";
//                fileName: "D:\\Test\\video\\akiyo_qcif.yuv";
                //                fileName: "D:\\Test\\video\\video.mp4";
                fileName: "D:\\Test\\video\\192.168.100.26_01_0_20170328_105837_1.mp4";
                state: Video.Stopped;

                MouseArea {
                    anchors.fill: parent;
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        //                        video1.setState(Video.Playing)
                        if(mouse.button == Qt.LeftButton)
                        {
                            console.log("pause")
                            parent.pause();
                        }
                        else
                        {
                            console.log("play")
                            parent.stop();
                            parent.play();
                        }
                    }
                }
            }
        }
    }

//        Rectangle {
//            width: 220;
//            height: 220;
//            anchors.bottom: parent.bottom;
//            anchors.right: parent.right;
//            color:"transparent";

//            AnimatedImage {
//                id: animated;
//                anchors.fill: parent;
//                source: "1.gif"
//            }
//        }



}
