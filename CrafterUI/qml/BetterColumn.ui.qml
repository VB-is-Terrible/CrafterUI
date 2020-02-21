import QtQuick 2.9
import QtQuick.Controls 2.9

Flickable {
    FontMetrics {
        id: fontMetrics
    }
    property alias column: column
    default property alias children: column.children
    boundsBehavior: Flickable.StopAtBounds
    contentHeight: column.height
    anchors.fill: parent
    ScrollBar.vertical: ScrollBar {
        id: vertibar

        policy: ScrollBar.AlwaysOn
    }
    Item {
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.rightMargin: vertibar.width + fontMetrics.maximumCharacterWidth * .2
        height: column.height
        Column {
            id: column
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            objectName: "column"
        }
    }
}
