import QtQuick 2.0
import QtQml.Models 2.15
import QtQuick.Controls 2.12
import BoardController 1.0

Rectangle {
    id: root

    anchors.margins: 20
    color: "khaki"

    signal mix()

    onMix: {
        boardModel.mix();
    }

    BoardModel {
        id: boardModel
        onWin: {
            victoryDialog.open()
        }
    }

    VictoryDialog {
        id: victoryDialog
        onVictory: mix()
    }

    Rectangle {
        anchors.margins: 20
        anchors.fill: parent
        border.color: Qt.darker("orange", 2)

        GridView {
            id: grid

            interactive: false
            model: boardModel

            anchors.fill: parent
            anchors.margins: 5
            cellHeight: height / boardModel.row()
            cellWidth: width / boardModel.column()

            displaced: Transition {
                id: animation
                NumberAnimation { properties: "x"; duration: 1000; property: "visible"}
                NumberAnimation { properties: "y"; duration: 1000 ; property: "visible"}
            }

            delegate: Cell {
                id: cell
                text: model.value
                width: grid.cellWidth
                height: grid.cellHeight
                opacity: (model.value !== 0) ? 1 : 0
                onItemCliced: {
                    if(boardModel.takeStep(index)){
                        //TODO Transition
                    }
                }
            }
        }
    }
}
