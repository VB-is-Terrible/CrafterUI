import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

Item {
    FontMetrics {
        id: fontMetrics
    }
    anchors.left: parent ? parent.left : undefined
    anchors.right: parent ? parent.right : undefined
    default property alias children: column.children
    property alias column: column
    property bool hasMethod: true
    property string methodName: qsTr("Put the method name here")
    implicitHeight: row.height + column.height + (hasMethod ? fontMetrics.height * .2 : 0)
    Row {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        id: row
        spacing: fontMetrics.averageCharacterWidth
        visible: hasMethod
        height: hasMethod ? fontMetrics.height : 0
        Text {
            text: qsTr("Method: ")
            font.underline: true
        }
        Text {
            text: methodName
            font.underline: true
        }
    }

    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: row.bottom
        anchors.topMargin: hasMethod ? fontMetrics.height * .2 : 0
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
