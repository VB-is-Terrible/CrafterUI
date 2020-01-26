import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

Item {
    property string name: qsTr("Item Name")
    property string count: qsTr("Item Count")
    anchors.rightMargin: 0
    anchors.leftMargin: 0
    anchors.left: parent.left
    anchors.right: parent.right
    height: 27

    Text {
        id: itemName
        objectName: "itemName"
        text: name
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
        id: itemCount
        objectName: "itemCount"
        text: count
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

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
