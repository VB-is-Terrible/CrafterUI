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
            y: 62
            anchors.left: rowLayout.anchors.left
            anchors.top: rowLayout.bottom
            height: 696
            currentIndex: pageIndicator.currentIndex
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 42
            clip: true
            Column {
                id: raw_page
                TableRow {}
                TableRow {}
                TableRow {}
                TableRow {}
                TableRow {}
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
