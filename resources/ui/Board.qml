import QtQuick
import QtQuick.Controls.Basic

import reversimodule 1.0

Rectangle {
    id: board
    property ReversiModel gameModel: ReversiModel {}

    property int cellPadding: 4
    property color boardColor: "#282d33"
    property color cellColor: "#7ca790"
    property int borderRadius: 10
    property int cellSize: (height - table.anchors.margins) / gameModel.rowCount(
                               ) - cellPadding

    color: boardColor
    radius: borderRadius

    TableView {
        id: table
        anchors.margins: 2 * cellPadding
        anchors.fill: parent
        clip: true

        model: gameModel
        columnSpacing: cellPadding
        rowSpacing: cellPadding
        boundsBehavior: Flickable.StopAtBounds

        delegate: Cell {
            implicitWidth: cellSize
            implicitHeight: cellSize
            borderRadius: board.borderRadius - table.anchors.margins
            backgroundColor: cellColor

            cellState: modelCellState // from model

            onClicked: {
                gameModel.trySetDisc(gameModel.index(row, column))
            }
        }
    }
}
