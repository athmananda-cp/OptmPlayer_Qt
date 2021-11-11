import QtQuick 2.2
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtGraphicalEffects 1.0

ApplicationWindow {
    id: rootWindow
    width: Screen.width
    height: Screen.height
    visible: true
    visibility: Window.FullScreen
    color: "#86c4eb"

    Rectangle {
        id: videoScreen
        width: rootWindow.width
        height: rootWindow.height - 80
        opacity: 0.3
    }

    Rectangle {
        width: rootWindow.width
        height: 80
        color: "black"
        opacity: 0.5
        anchors.top: videoScreen.bottom
    }

    // ----------------------------------------------------------------------------
    // This section is temporary. At present the binary is taken from local folder
    // and not fetched from other device. This timer will show the update Path
    // dialog after one second of main screen display.
    Timer {
        interval: 1000
        repeat: false
//        running: true
        onTriggered: {
            var updtePath = Qt.createComponent("qrc:/qml/UpdatePathDialog.qml")
            var object = updtePath.createObject(rootWindow)
        }
    }
    // ----------------------------------------------------------------------------

    // ----------------------------------------------------------------------------
    // This connection will get notification about SW update availability. On this
    // an Update Dialog should be shown which will continue with the work flow.
    Connections {
        target: swUpdateManager
        onSwUpdateAvailable : {
//            console.log("-------------------> Received the signal upto here ....")
//            console.log("-----> ", swUpdateInfoViewModel.title)
//            console.log("-----> ", swUpdateInfoViewModel.description)
//            console.log("-----> ", swUpdateInfoViewModel.enhancements)
            var updteInfo = Qt.createComponent("qrc:/qml/UpdateInfoDialog.qml")
            var object = updteInfo.createObject(rootWindow)
            object.listOfChanges = swUpdateInfoViewModel.enhancements
        }
    }
    // ----------------------------------------------------------------------------
}
