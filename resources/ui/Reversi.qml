import QtQuick
import QtQuick.Window
import QtQuick.Controls.Basic
import QtQuick.Shapes
import QtQuick.Layouts

import reversimodule 1.0

Window {
    id: window

    readonly property int windowPadding: 40
    readonly property color backgroundColor: "#6bb88f"
    readonly property color boardColor: "#282d33"
    readonly property color colorWhite: "#f5f4e4"

    width: 700
    height: 800

    minimumWidth: 400
    minimumHeight: 400

    visible: true
    title: qsTr("Reversi")
    color: backgroundColor

    FontLoader {
        id: appFont
        source: "../fonts/FredokaOne-Regular.ttf"
    }

    ReversiModel {
        id: rmodel
        property alias rmodel: rmodel
    }

    Timer {
        id: timer
        property alias timer: timer
        interval: 1000
        running: /*false*/ true
        repeat: true
        onTriggered: {
            var textTimer = timerText.text
            textTimer = textTimer.replace(':', '')
            var time = Number(textTimer)
            var minutes = Math.floor(time / 100)
            var seconds = time % 100
            minutes = seconds == 59 ? minutes + 1 : minutes
            seconds = seconds == 59 ? 0 : seconds + 1
            timerText.text = qsTr(('0' + minutes).slice(
                                      -2) + ':' + ('0' + seconds).slice(-2))
        }
    }

    Component {
        id: gameMessage
        Text {
            id: playerName
            text: rmodel.gameMessage
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 24
            font.bold: true
            font.family: appFont.name
            color: boardColor
            fontSizeMode: Text.HorizontalFit
        }
    }

    Component {
        id: restartButton
        Button {
            id: button
            flat: true
            background: Rectangle {
                anchors.fill: button
                radius: board.borderRadius
                color: button.pressed ? Qt.darker(
                                            boardColor,
                                            1.5) : button.hovered ? Qt.lighter(
                                                                        boardColor,
                                                                        2) : boardColor
            }

            contentItem: Text {
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("Restart")
                font.pointSize: 14
                font.bold: true
                font.family: appFont.name
                color: colorWhite
            }

            onClicked: {
                timer.restart()
                timer.stop()
                timerText.text = qsTr("00:00")
                timer.interval = 1000
                timer.start()
                rmodel.restart()
            }
        }
    }

    ColumnLayout {
        id: columnLayout
        anchors.margins: windowPadding
        anchors.fill: parent
        spacing: 20

        Board {
            id: board
            gameModel: rmodel

            Layout.preferredWidth: Math.min(
                                       columnLayout.width,
                                       columnLayout.height - bottomPanel.height
                                       - columnLayout.spacing)
            Layout.preferredHeight: width

            Layout.alignment: Qt.AlignCenter

            cellColor: backgroundColor
            boardColor: boardColor
        }

        RowLayout {
            id: bottomPanel
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom

            Layout.minimumHeight: 82 / 2
            Layout.maximumHeight: 82 / 2

            // Stopwatch
            Rectangle {
                id: stopwatch
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                implicitWidth: 2 * height
                Layout.fillHeight: true
                color: colorWhite
                radius: board.borderRadius

                Text {
                    id: timerText
                    property alias timerText: timerText
                    text: qsTr("00:00")
                    anchors.centerIn: parent
                    font.pointSize: 14
                    font.bold: true
                    font.family: appFont.name
                    color: boardColor
                }
            }

            Loader {
                sourceComponent: gameMessage

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            }

            Loader {
                sourceComponent: restartButton
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            }
        }
    }
}
