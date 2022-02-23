import QtQuick 2.0
//import QtQuick.Controls 2.15

Rectangle {
    id: button
    property string buttonText: ""
    signal buttonClicked;

    border.width: 2
    border.color: "#fdfbfb"
    radius: 5
    color: "transparent"
    anchors.margins: 10

    Text {
        id: text2
        anchors.fill: parent
        color: "#fdfbfb"
        text: buttonText
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            button.buttonClicked()
        }

        onEntered: {
            button.color = "skyblue"
        }

        onExited: {
            button.color = "transparent"
        }
    }
}
