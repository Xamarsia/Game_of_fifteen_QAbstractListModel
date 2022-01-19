import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.12

Window {
    id: root;

    width: 500
    height: 500
    visible: true
    title: qsTr("Game of Fifteen")

    Background{
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: 2

            Board {
                id: board

                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            MixButton {
               Layout.alignment: Qt.AlignHCenter 


               Dialog {
                   id: restartGameDialog

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
                        restartGameDialog.close()
                   }
               }
               onCliced: {
                   restartGameDialog.open()
               }
            }
        }
    }
}



