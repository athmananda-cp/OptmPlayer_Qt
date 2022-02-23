/*
* VLC-Qt QML Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
*/

import QtQuick 2.0
import VLCQt 1.0
import QtMultimedia 5.0

Rectangle {
    width: 640
    height: 480
    color: "black"

    VlcVideoPlayer {
        id: vidwidget
        anchors.fill: parent
        url: "http://devimages.apple.com/iphone/samples/bipbop/bipbopall.m3u8"
    }
    VideoOutput {
        id: videoOutput
        source: vidwidget
        anchors.fill: parent
    }
}
