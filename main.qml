import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: root

    property var clicked: false
    property var index: -1

    title: qsTr("Match 3")
    width: 800
    height: 820
    visible: true

    Rectangle {

        id:mainRect

        anchors.fill: parent

        color: "grey"
        border.color: "black"

        GridView {

            id: view

            anchors.fill: parent
            cellHeight:  mainRect.height  / myModel.config.rows
            cellWidth:   mainRect.width / myModel.config.columns

            interactive: false
            focus: true
            snapMode: GridView.SnapToRow
            verticalLayoutDirection: GridView.BottomToTop

            model: myModel
            move: Transition {

                ParallelAnimation {
                    NumberAnimation {
                        easing.type: Easing.OutCubic
                        properties: "y"
                        duration: 500
                        alwaysRunToEnd: true
                    }
                    NumberAnimation {
                        easing.type: Easing.OutCubic
                        properties: "x"
                        duration: 500
                        alwaysRunToEnd: true
                    }
                }
            }
            moveDisplaced: Transition {
                NumberAnimation {
                    easing.type: Easing.OutCubic; properties: "x, y"; duration: 500
                    alwaysRunToEnd: true
                }
            }
            delegate: Component {
                Item {
                    id: item

                    width: view.cellWidth
                    height: view.cellHeight

                    Image {
                        id: iconLoader

                        property bool scaleProperty: false
                        anchors.centerIn: parent
                        width: item.width * 0.7
                        height: item.height * 0.7

                        source: path
                        opacity: flag ? 0 : 1

                        Behavior on opacity { NumberAnimation {duration: 800} }
                        Text {
                            anchors.centerIn: parent
                            text: index + " " + name
                        }
                    }

                    SequentialAnimation {
                        id: scaleAnim
                        loops: Animation.Infinite
                        alwaysRunToEnd: true
                        running: root.clicked && (index === root.index)
                        NumberAnimation {
                            from: 1
                            to: 1.3
                            target: iconLoader
                            property: "scale"
                            duration: 300
                            easing.type: Easing.InOutQuad
                        }
                        NumberAnimation {
                            from: 1.3
                            to: 1
                            target: iconLoader
                            property: "scale"
                            duration: 300
                            easing.type: Easing.InOutQuad
                        }
                    }

                    MouseArea {

                        anchors.fill: parent
                        onClicked: {
                            view.currentIndex = index;

                            if (root.clicked && (index != root.index)) {
                                root.clicked = false;
                                if (myModel.config.moves < myModel.config.maxMoves) {
                                    root.index = myModel.swapTwoElements(root.index, index);
                                }
                                if (root.index != -1 && root.index != -2) {
                                    root.clicked = true;
                                }
                            }
                            else if (!root.clicked){
                                root.clicked = true;
                                root.index = index;
                            }
                        }
                    }
                }
            }
        }
    }
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&New game")
                onTriggered: myModel.newGame();
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }
    statusBar: StatusBar {
        id: statusBar

        width: parent.width
        height: 20

        Row {
            anchors.fill: parent
            Label { text: "Score: " }
            Label {id: currentScore; text: myModel.config.score}
            Label { text: "       Moves Left: "}
            Label { id: movesLeft; text: myModel.config.maxMoves - myModel.config.moves}
            Label { text: "       Minimum Score: "}
            Label { id: minScore; text: myModel.config.minScore}
        }
    }
    MessageDialog {
        id: messageDialog

        property bool isVictory: myModel.config.isVictory
        property bool  movesNotAvailable: myModel.config.moves === myModel.config.maxMoves

        title: movesNotAvailable ? qsTr("Try again") : qsTr("Victory")
        visible: isVictory || movesNotAvailable
        text: isVictory ? "Level Completed" : "Level failed"
        onAccepted: myModel.newGame();
    }
}
