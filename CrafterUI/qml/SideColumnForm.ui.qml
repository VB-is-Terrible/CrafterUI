import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

Item {
    FontMetrics {
        id: fontMetrics
    }

    height: 200
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
    }
}
