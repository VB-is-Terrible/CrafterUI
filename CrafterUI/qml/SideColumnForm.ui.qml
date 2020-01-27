import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

Item {
    FontMetrics {
        id: fontMetrics
    }

    height: 800
    width: 18 * fontMetrics.maximumCharacterWidth

    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.minimumWidth: 10 * fontMetrics.maximumCharacterWidth
    Layout.maximumWidth: Screen.desktopAvailableWidth * .2
    Layout.preferredWidth: 22 * fontMetrics.maximumCharacterWidth
    Layout.preferredHeight: 600
    Layout.minimumHeight: 300

    Rectangle {
        id: rectangle1
        anchors.fill: parent
        color: 'plum'
        Label {
            text: qsTr("You are on the side bar.")
            anchors.centerIn: parent
        }

        RowLayout {
            id: rowLayout
            height: 40
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10

            Button {
                id: button
                text: qsTr("New Request")
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Button {
                id: button1
                text: qsTr("Raw Materials")
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        SwipeView {
            id: mainView
            objectName: "mainView"
            y: 62
            anchors.left: rowLayout.anchors.left
            anchors.top: rowLayout.bottom
            height: 696
            anchors.rightMargin: 10
            currentIndex: pageIndicator.currentIndex
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 42
            clip: true
            Item {
                Flickable {
                    id: scrollView
                    anchors.fill: parent
                    contentHeight: rawPage.height
                    boundsBehavior: Flickable.StopAtBounds
                    ScrollBar.vertical: ScrollBar {
                        id: vertibar
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        policy: ScrollBar.AlwaysOn
                    }
                    Item {
                        anchors.right: parent.right
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.rightMargin: vertibar.width + 3
                        height: rawPage.height
                        Column {
                            id: rawPage
                            anchors.right: parent.right
                            anchors.left: parent.left
                            anchors.top: parent.top
                            objectName: "rawPage"
                            TableRow {}
                            TableRow {}
                            TableRow {}
                            TableRow {}
                        }
                    }
                }
            }
        }

        PageIndicator {
            id: pageIndicator
            height: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            count: mainView.count
            currentIndex: mainView.currentIndex
        }
    }
}

/*##^##
Designer {
    D{i:4;anchors_width:100;anchors_x:23;anchors_y:16}D{i:7;anchors_width:646}
}
##^##*/

