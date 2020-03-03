import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

Item {
    FontMetrics {
        id: fontMetrics
    }
    id: element
    TextArea {
        id: recipeInput
        anchors.bottomMargin: fontMetrics.height
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: baseRow.top
        placeholderText: "Recipe Name: Amount"
        objectName: "recipeInput"
    }
    RowLayout {
        id: baseRow
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.rightMargin: fontMetrics.height * .5
        anchors.leftMargin: fontMetrics.height * .5
        anchors.bottomMargin: fontMetrics.height * .5
        Button {
            id: accept
            text: qsTr("Accept")
            Layout.fillWidth: true
            Layout.fillHeight: true
            //            Keys.onReturnPressed: accept_change();
            //            Keys.onEnterPressed: accept_change();
        }
        Button {
            id: cancel
            text: qsTr("Cancel")
            Layout.fillWidth: true
            Layout.fillHeight: true
            //            Keys.onReturnPressed: cancel_change();
            //            Keys.onEnterPressed: cancel_change();
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
