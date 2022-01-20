import QtQuick 2.0
import QtQuick.Controls 2.12

Dialog {
    id: restartGameDialog

    signal restart()

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    parent: Overlay.overlay
    modal: true
    title: "Restart the game"
    standardButtons: Dialog.Close | Dialog.Reset

    Label {
        text: "Are you sure you want to restart the game?"
    }
    onReset: {
         restart()
         restartGameDialog.close()
    }
}
