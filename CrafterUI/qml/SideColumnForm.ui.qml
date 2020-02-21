import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

Item {
    FontMetrics {
        id: fontMetrics
    }

    height: 800
    property alias button1: button1
    property alias mainView: mainView
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
            anchors.centerIn: parent
        }

        RowLayout {
            id: rowLayout
            height: fontMetrics.height * 2
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
            anchors.topMargin: fontMetrics.maximumCharacterWidth * .2
            height: 696
            anchors.rightMargin: 10
            currentIndex: pageIndicator.currentIndex
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 42
            clip: true
            interactive: false
            Item {
                BetterColumn {
                    column {
                        id: rawPage
                        objectName: "rawPage"
                    }

                }
            }
            Item {
                RecipeDetailed {
                    id: recipeDetailed
                    objectName: "recipeDetailed"
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
