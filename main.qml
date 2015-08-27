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


        Component {

            id: highlight

            Rectangle {

                width: view.cellWidth
                height: view.cellHeight

                x: view.currentItem.x
                y: view.currentItem.y
                color: "lightsteelblue"
                radius: 5

                Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
            }
        }

        GridView {

            id: view
            anchors.fill: parent
            cellHeight:  mainRect.height  / myModel.config.rows
            cellWidth:   mainRect.width / myModel.config.columns
            interactive: false

            highlight: highlight
            highlightFollowsCurrentItem: false
            focus: true

            model: myModel
            snapMode: GridView.SnapToRow
            verticalLayoutDirection: GridView.BottomToTop

            move: Transition {

                NumberAnimation {
                    id: anim
                    easing.type: Easing.OutCubic; properties: "y"; duration: 500
                    alwaysRunToEnd: true
                }
                onRunningChanged:
                {
                    if (!anim.running) {
                        // stop
                        //    myModel.searchForMatch();
                        //     console.log("fgdgf");
                    } else {
                        // start
                        //  console.log("aaaaa");
                    }

                }
            }
            moveDisplaced: Transition {

                NumberAnimation {
                    id: anim1
                    easing.type: Easing.OutCubic; properties: "y"; duration: 500
                    alwaysRunToEnd: true
                }
                onRunningChanged:
                {
                    if (!anim1.running) {
                        // stop
                        //     myModel.searchForMatch();
                        //     console.log("fgdgf");
                    } else {
                        // start
                        //  console.log("aaaaa");
                    }

                }
            }

            delegate: Component {

                Rectangle {
                    id: item
                    width: view.cellWidth
                    height: view.cellHeight
                     color: myModel.flag ? "red" : mainRect.color
                    Image {
                        id: iconLoader
                        anchors.centerIn: parent
                        source: model.path
                        cache: false
                        width: item.width * 0.7
                        height: item.height * 0.7
                        Text {
                            anchors.centerIn: parent
                            text: index + " " + model.name
                            color:"white"
                        }
                        onSourceChanged: {
                            //  console.log("Source: " + source + " index: " + index);
                        }
                    }
                    Behavior on color {  ColorAnimation { duration: 200 }}
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            //console.log("@@@CLICK: " + iconLoader.source + " index: " + index + " Name: " + model.name);
                            view.currentIndex = index;
                            // console.log("current name: " + myModel.getName(index));

                            if (root.clicked && (index != root.index)) {
                                myModel.swapTwoElements(root.index, index);
                                root.clicked = false;
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
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
