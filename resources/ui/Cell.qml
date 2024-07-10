import QtQuick 2.12
import QtQuick.Controls 2.12

import reversimodule 1.0

AbstractButton {
    property color backgroundColor: "#7ca790"

    property color currentBackgroundColor: hovered && cellState
                                           === ReversiModel.Empty ? Qt.lighter(
                                                                        backgroundColor,
                                                                        1.2) : backgroundColor
    property double borderRadius: 0.1
    property int cellState: ReversiModel.Empty
    property url lightStateUrl: "../icons/LightDisk.svg"
    property url darkStateUrl: "../icons/DarkDisk.svg"
    property url emptyStateUrl: ""

    background: Rectangle {
        anchors.fill: parent
        color: currentBackgroundColor
        radius: borderRadius
    }

    Image {
        id: image
        source: getImageUrl()
        x: parent.width * 0.1
        y: x
        width: parent.width - 2 * x
        height: parent.height - 2 * x
        antialiasing: true
        smooth: true
    }

    onCellStateChanged: {
        image.source = getImageUrl()
        console.log(getImageUrl())
    }

    function getImageUrl() {
        if (cellState === ReversiModel.Empty)
            return emptyStateUrl

        return cellState === ReversiModel.White ? lightStateUrl : darkStateUrl
    }
}
