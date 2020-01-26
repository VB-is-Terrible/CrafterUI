import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

Item {
    FontMetrics {
        id: fontMetrics
    }

    property string itemName: qsTr("Item Name")
    property string itemCount: qsTr("Item Count")
    anchors.rightMargin: 0
    anchors.leftMargin: 0
    anchors.left: parent ? parent.left : undefined
    anchors.right: parent ? parent.right : undefined
    height: 27

    Text {
        id: nameDisplay
        objectName: "nameDisplay"
        text: itemName
        anchors.right: itemCount.left
        anchors.rightMargin: 573
        fontSizeMode: Text.Fit
        verticalAlignment: Text.AlignVCenter
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
    }

    Text {
        id: countDisplay
        objectName: "countDisplay"
        text: itemCount
        horizontalAlignment: Text.AlignRight
        elide: Text.ElideRight
        fontSizeMode: Text.Fit
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 10 * fontMetrics.averageCharacterWidth
    }
}



