import QtQuick 2.0
import QtQml.Models 2.15
import QtQuick.Controls 2.12

Rectangle {
    id: root

    anchors.margins: 20
    color: "khaki"

    Rectangle {
        anchors.margins: 20
        anchors.fill: parent
        border.color: Qt.darker("orange", 2)

        GridView {
            id: grid

            interactive: false
            model: 16

            anchors.fill: parent
            anchors.margins: 5
            cellHeight: height / 4
            cellWidth: width / 4

            move: Transition {
                NumberAnimation { properties: "x"; duration: 500 }
                NumberAnimation { properties: "y"; duration: 500 }
            }

            Dialog {
                id: victoryDialog

                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                parent: Overlay.overlay
                modal: true
                title: "Restart the game"
                standardButtons: Dialog.Close | Dialog.Reset

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
                     victoryDialog.close()
                }
            }

            delegate: Cell {
                id: cell
                width: grid.cellWidth
                height: grid.cellHeight
            }
        }
    }
}
