import QtQuick 2.0

Item {
    id:updateErrorDialog
    width: 360
    height: 120
    property alias text1: text1
    anchors.centerIn: parent

    property string listOfError: ""

    Rectangle {
        id: rectangle
        x: 5
        y: 0
        color: "#35b6ce"
        opacity: 0.9
        radius: 15
            anchors.rightMargin: 2
            anchors.bottomMargin: 0
            anchors.leftMargin: 2
            anchors.topMargin: -47
            border.width: 2
            anchors.fill: parent

            Text {
                id: text1
                x: 0
                y: 7
                width: 360
                height: 30
                color: "#0e0c0c"
                text: qsTr("")
                font.weight: Font.DemiBold
                font.capitalization: Font.Capitalize
                font.family: "Verdana"
                font.pixelSize: 40
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                id:errorText
                x: 29
                y: 61
                width: 308
                height: 51
                color: "#1d1919"
                text: listOfError
                font.pixelSize: 20
            }

            PlayerButton {
                x: 241
                y: 109
                width: 107
                height: 50
                color: "#137595"
                border.color: "#151313"
                border.width: 2
                buttonText: "Exit"
                onButtonClicked: {
                    updateErrorDialog.visible = false
                    player.exit()
                }
            }
    }


}
