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
    property int makes: 1
    implicitHeight: row.height + column.height + (hasMethod ? fontMetrics.height * .2 : 0)
    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        id: row
        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: fontMetrics.averageCharacterWidth
            Text {
                text: qsTr("Makes: ")
                font.underline: true
            }
            Text {
                text: makes
                font.underline: true
            }
        }
        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: fontMetrics.averageCharacterWidth
            visible: hasMethod
            height: hasMethod ? fontMetrics.height : 0
            Text {
                text: qsTr("Method: ")
            }
            Text {
                text: methodName
            }
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
