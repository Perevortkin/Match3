import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: root

    property var clicked: false
    property var index: 0

    title: qsTr("Hello World")
    width: 900
    height: 900
    visible: true

    Rectangle {

        id:mainRect

        anchors.fill: parent

        color: "grey"
        border.color: "black"


        Component {
            id: highlight
            Rectangle {
                width: view.cellWidth; height: view.cellHeight
                color: "lightsteelblue"; radius: 5
                x: view.currentItem.x
                y: view.currentItem.y
                Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
            }
        }

        GridView {

            id: view
            anchors.fill: parent
            cellHeight:  mainRect.height / myConfig.rows
            cellWidth:   mainRect.width / myConfig.columns
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

                Item {
                    id: item
                    width: view.cellWidth
                    height: view.cellHeight
                    Image {
                        id: iconLoader
                        anchors.centerIn: parent
                        source: model.path
                        cache: false
                        Text {
                            anchors.centerIn: parent
                            text: index + " " + model.name
                            color:"white"
                        }
                        onSourceChanged: {
                          //  console.log("Source: " + source + " index: " + index);
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
    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
