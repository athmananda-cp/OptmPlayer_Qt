import QtQuick 2.4

Item {
    id:updateInfoDialog
    width: 480
    height: 320
    anchors.centerIn: parent

    property string listOfChanges: ""

    Rectangle {
        id: rectangle
        opacity: 0.9
        color: "#303031"
        radius: 15
        border.width: 0
        anchors.fill: parent

        Text {
            x: 0
            y: 19
            width: parent.width
            height: 44
            color: "#fdfbfb"
            text: qsTr("NEW UPDATE AVAILABLE")
            font.pixelSize: 25
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            x: 20
            y: 60
            width: parent.width - 40
            height: 40
            color: "#fdfbfb"
            text: qsTr("List of changes")
            font.pixelSize: 20
            font.bold: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id:changesText
            x: 20
            y: 106
            width: 440
            height: 150
            color: "#fdfbfb"
            text: listOfChanges
            font.pixelSize: 20
        }

        PlayerButton {
            id: cancelButton
            x: 149
            y: 260
            width: 150
            height: 50
            radius: 1
            buttonText: "Do it later"

            onButtonClicked: {
                updateInfoDialog.visible = false
            }
        }

        PlayerButton {
            id: openButton
            x: 310
            y: 260
            width: 150
            height: 50
            radius: 1
            buttonText: "Update now"
            onButtonClicked: {
                updateInfoDialog.visible = false
                var updteInfo = Qt.createComponent("qrc:/qml/UpdateProgressDialog.qml")
                var object = updteInfo.createObject(rootWindow)
                swUpdateManager.downloadUpdate()
            }
        }
    }
}
