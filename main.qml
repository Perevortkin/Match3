import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: root

    property var clicked: false
    property var index: 0

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

            model: myModel
            snapMode: GridView.SnapToRow
            verticalLayoutDirection: GridView.BottomToTop

            move: Transition {

                NumberAnimation {
                    id: anim
                    easing.type: Easing.OutCubic; properties: "x, y"; duration: 500
                    alwaysRunToEnd: true
                }
                onRunningChanged:
                {
                    if (!anim.running) {
                        // stop
                        //    myModel.searchForMatch();
                        //console.log("AnimationStopped");
                    } else {
                        // start
                        //console.log("AnimationStatted");
                    }

                }
            }
            moveDisplaced: Transition {

                NumberAnimation {
                    id: anim1
                    easing.type: Easing.OutCubic; properties: "x, y"; duration: 500
                    alwaysRunToEnd: true
                }
                onRunningChanged:
                {
                    if (!anim1.running) {
                        // stop
                        //     myModel.searchForMatch();
                    } else {
                        // start
                        //  console.log("aaaaa");
                    }

                }
            }

            delegate: Component {

                Item {
                    id: item
                    width: view.cellWidth
                    height: view.cellHeight

                    Image {
                        id: iconLoader
                        anchors.centerIn: parent
                        source: model.path
                        width: item.width * 0.7
                        height: item.height * 0.7

                        Text {
                            anchors.centerIn: parent
                            text: index + " " + model.name
                            color:"black"
                                //color: myModel.flag ? "red" : mainRect.color
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            //console.log("@@@CLICK: " + iconLoader.source + " index: " + index + " Name: " + model.name);
                            view.currentIndex = index;
                            // console.log("current name: " + myModel.getName(index));
                            if (root.clicked && (index != root.index)) {
                                myModel.swapTwoElements(root.index, index);
                                root.clicked = false;
                                if (myModel.config.isVictory) {
                                    messageDialog.show("Level Completed");
                                }
                            }
                            else {
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
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
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
        title: qsTr("Victory")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
