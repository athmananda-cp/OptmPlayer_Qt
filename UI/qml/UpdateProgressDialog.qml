import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id:updateProgressDialog
    width: 1000
    height: 250
    anchors.centerIn: parent

    property string titleText: ""
    property string completText: ""
    property string remainingText: ""
    property real progress: 0

    state: "DownloadingUpdate"
    states: [
        State {
            name: "DownloadingUpdate"
            PropertyChanges { target: updateProgressDialog; titleText: swUpdateDownloadViewModel.title}
            PropertyChanges { target: updateProgressDialog; completText: swUpdateDownloadViewModel.completionStatus}
            PropertyChanges { target: updateProgressDialog; remainingText: swUpdateDownloadViewModel.remainingStatus}
            PropertyChanges { target: updateProgressDialog; progress: swUpdateDownloadViewModel.downloadedPercent}
        },
        State {
            name: "InstallingUpdate"
            PropertyChanges { target: updateProgressDialog; titleText: swUpdateInstallViewModel.title}
            PropertyChanges { target: updateProgressDialog; completText: ""}
            PropertyChanges { target: updateProgressDialog; remainingText: swUpdateInstallViewModel.remainingStatus}
            PropertyChanges { target: updateProgressDialog; progress: swUpdateInstallViewModel.installedPercent}
        }
    ]

    Connections {
        target: swUpdateManager
        onSwUpdatedDownloadStarted       : {state = "DownloadingUpdate"}
        onSwUpdateDownloadFailed         : {}
        onSwUpdateDownloadCompleted      : {}
        onSwUpdateInstallationStarted    : {state = "InstallingUpdate"}
        onSwUpdateInstallationFailed     : {}
        onSwUpdateInstallationCompleted  : {
            updateProgressDialog.visible = false
            var updteCompletd = Qt.createComponent("qrc:/qml/UpdateCompleteDialog.qml")
            var object = updteCompletd.createObject(rootWindow)
        }
    }

    Rectangle {
        id: rectangle
        opacity: 0.9
        color: "#303031"
        radius: 15
        border.width: 0
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        Text {
            id: title
            x: 20
            y: 19
            width: 961
            height: 44
            color: "#fdfbfb"
            font.pixelSize: 25
            font.bold: true
            text: titleText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            x: 61
            y: 81
            width: 515
            height: 44
            color: "#fdfbfb"
            text: completText
            font.pixelSize: 20
        }

        ProgressBar {
            id:control
            value: progress / 100
            x: 61
            y: 124
            width: 879
            height: 30

            background: Rectangle {
                implicitWidth: 200
                implicitHeight: 6
                color: "#e6e6e6"
                radius: 15
            }

            contentItem: Item {
                implicitWidth: 200
                implicitHeight: 4

                Rectangle {
                    width: control.visualPosition * parent.width
                    height: parent.height
                    radius: 15
                    color: "#3264a8"
                }
            }
        }

        Text {
            x: 425
            y: 198
            width: 515
            height: 44
            color: "#fdfbfb"
            text: remainingText
            font.pixelSize: 20
            horizontalAlignment: Text.AlignRight
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.1}
}
##^##*/
