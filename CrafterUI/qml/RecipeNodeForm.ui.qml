import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12

Rectangle {
    id: element1
    width: 400
    height: 200
    property alias fontMetrics: fontMetrics
    property alias mouseArea: mouseArea
    property string contents: qsTr("1\t: Test Input\n2\t: Carpenter Recipe\n30\t: Liquid Redstone\n\n\n\n\n\n\n\n\n12\t: Stone Slabs\n10000\t: Really long recipe name that's meant to overflow the text area for testing purposes. Is this long enough?")
    property string heading: qsTr("Recipe name")
    property bool highlight: false
    border.width: highlight ? 3 : 1
    border.color: highlight ? "red" : "black"
    FontMetrics {
        id: fontMetrics
    }

    MouseArea {
        id: mouseArea
        z: 100
        anchors.fill: parent
    }

    Text {
        height: 21
        text: heading
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        elide: Text.ElideRight
        textFormat: Text.PlainText
        verticalAlignment: Text.AlignTop
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 16
    }
    ScrollView {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 35
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        TextArea {
            text: contents
            anchors.rightMargin: 6
            anchors.bottomMargin: 6
            anchors.leftMargin: -6
            anchors.topMargin: -6
            anchors.fill: parent
            readOnly: true
        }
    }
}

/*##^##
Designer {
    D{i:1;anchors_width:134;anchors_x:8;anchors_y:8}D{i:2;anchors_height:157;anchors_width:184;anchors_x:8;anchors_y:35}
D{i:3;anchors_x:32;anchors_y:68}
}
##^##*/

