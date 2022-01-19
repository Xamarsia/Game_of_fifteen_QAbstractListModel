import QtQuick 2.0

Rectangle {
    id: root

    property alias text: label.text

    signal itemCliced();

    color: "#00ffffff"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.itemCliced()
        }
    }

    Rectangle {
        radius: 10
        color: "orange"
        anchors.margins: 3
        anchors.fill: parent
        border.color: Qt.darker("orange", 2)

        Text {
            id: label

            color: "black"
            font.pixelSize: 24
            anchors.centerIn: parent
        }
    }
}

