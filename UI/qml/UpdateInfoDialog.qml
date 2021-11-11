import QtQuick 2.4

Item {
    id:updateInfoDialog
    width: 600
    height: 500
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
            y: 80
            width: parent.width - 40
            height: 44
            color: "#fdfbfb"
            text: qsTr("List of changes")
            font.pixelSize: 20
            font.bold: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id:changesText
            x: 25
            y: 130
            width: 550
            height: 289
            color: "#fdfbfb"
            text: listOfChanges
            font.pixelSize: 20
        }

        PlayerButton {
            id: cancelButton
            x: 41
            y: 436
            width: 250
            height: 50
            buttonText: "Do it later"

            onButtonClicked: {
                updateInfoDialog.visible = false
            }
        }

        PlayerButton {
            id: openButton
            x: 310
            y: 436
            width: 250
            height: 50
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
