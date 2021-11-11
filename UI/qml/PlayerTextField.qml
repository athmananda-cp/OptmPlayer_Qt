import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle {
    id:textFiled
    property string placeholderText: ""
    property string text: textField.text

    border.width: 2
    border.color: "#fdfbfb"
    radius: 5
    color: "transparent"
    anchors.margins: 10

    TextField {
        id: textField
        color: "#fdfbfb"
        placeholderText: textFiled.placeholderText
        font.pixelSize: 20
        width: parent.width - 10
        height: parent.height - 10
        anchors.centerIn: parent
        clip: true
        background: Item { opacity: 0 }
    }
}
