import QtQuick 2.4

Item {
    id:updateCompleteDialog
    width: 480
    height: 200
    anchors.centerIn: parent

    property string listOfChanges: ""

    Rectangle {
        id: rectangle
        opacity: 0.9
        color: "#303031"
        radius: 15
        border.width: 0
        anchors.fill: parent
        anchors.bottomMargin: 0

        Text {
            x: 0
            y: 19
            width: parent.width
            height: 44
            color: "#fdfbfb"
            text: qsTr("INSTALLATION COMPLETE")
            font.pixelSize: 25
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        PlayerButton {
            x: 30
            y: 122
            width: 200
            height: 50
            radius: 6
            border.width: 6
            buttonText: "Reboot Later"

            onButtonClicked: {
                updateCompleteDialog.visible = false
            }
        }

        PlayerButton {
            x: 265
            y: 122
            width: 200
            height: 50
            border.width: 6
            buttonText: "Reboot Now"
            onButtonClicked: {
                updateCompleteDialog.visible = false
                player.exit()
            }
        }
    }
    states: [
        State {
            name: "State1"
        }
    ]
}
