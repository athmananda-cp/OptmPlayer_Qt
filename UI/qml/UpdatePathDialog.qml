import QtQuick 2.4

Item {
    id:updateDialog
    width: 640
    height: 240
    anchors.centerIn: parent

    Rectangle {
        id: rectangle
        opacity: 0.9
        color: "#303031"
        radius: 15
        border.width: 0
        anchors.fill: parent

        Text {
            x: 20
            y: 19
            width: 961
            height: 44
            color: "#fdfbfb"
            text: qsTr("Enter a path or find a file on your system")
            font.pixelSize: 25
            font.bold: true
        }

        Text {
            x: 729
            y: 89
            width: 30
            height: 44
            color: "#fdfbfb"
            text: qsTr("or")
            font.pixelSize: 20
        }

        PlayerTextField {
            id:pathTextFiled
            x: 20
            y: 78
            width: 682
            height: 50
            placeholderText: "Enter path here"
        }

        PlayerButton {
            id: fileDialogButton
            x: 776
            y: 78
            width: 200
            height: 50
            buttonText: "Find on your system"

            onButtonClicked: {
                console.log("Find Clicked")
            }
        }

        PlayerButton {
            id: cancelButton
            x: 559
            y: 174
            width: 200
            height: 50
            buttonText: "Cancel"

            onButtonClicked: {
                updateDialog.visible = false
            }
        }

        PlayerButton {
            id: openButton
            x: 776
            y: 174
            width: 200
            height: 50
            buttonText: "Open"
            onButtonClicked: {
                console.log("Open Clicked : ", pathTextFiled.text)
            }
        }
    }
}
