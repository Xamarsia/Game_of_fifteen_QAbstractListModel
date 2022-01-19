import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle {
    id: root

    signal cliced();

    width: 160
    height: 84
    color: "khaki"

    Button {
        id: mixButton
        width: 140
        height: 64
        text: qsTr("Mix")
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.bottomMargin: 20

        background: Rectangle {
            radius: 45
            anchors.fill: parent
            border.color: Qt.darker("orange", 2)
            color: parent.hovered ? "darkorange" : "orange"
        }
        onClicked: root.cliced();
    }
}
