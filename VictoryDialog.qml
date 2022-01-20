import QtQuick 2.0
import QtQuick.Controls 2.12

Dialog {
    id: victoryDialog

    signal victory()

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    parent: Overlay.overlay
    modal: true
    title: "Restart the game"
    standardButtons: Dialog.Reset

    Column {
        anchors.fill: parent
        spacing: 4
        Label {
            text: "Сongratulations!"
            color: "red"
            font.pixelSize: 24
        }
        Label {
            text: "You are a winner"
            font.pixelSize: 18
        }
    }

    onReset: {
         victory()
         victoryDialog.close()
    }
}
