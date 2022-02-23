import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtGraphicalEffects 1.0
import VLCQt 1.0
import QtMultimedia 5.0

Item {
    id:playerWindow
    width: 480
    height: 200
    anchors.centerIn: parent
    property string playbackurl: ""
    //"192.168.1.2/hls/56-5003/index.m3u8"

    //console.log("-------------------> Received the signal upto here ....",playbackurl)
    Rectangle {
        id: vidwidget
        width: 720
        height: 480
        color: "skyblue"
        visible: true
        opacity: 100

        VlcVideoPlayer {
            anchors.fill: parent
            url: playbackurl
        }
    }

    Rectangle {
        width: 720
        height: 80
        color: "black"
        opacity: 1
        anchors.top: vidwidget.bottom
    }

}
