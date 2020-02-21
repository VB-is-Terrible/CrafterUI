import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12

Item {
    FontMetrics {
        id: fontMetrics
    }
    clip: true
    id: element
    property string recipeName: qsTr("Your recipe name here")
    property var recipeValues: ["The", "names", "of", "recipes", "here"]
    width: 400
    height: 400
    property alias recipeSelector: recipeSelector
    property bool isDefaultRecipe: false
    property int recipeCount: -1
    property alias cancel: cancel
    property alias accept: accept
    property alias swiper: swiper
    property alias spinBox: spinBox
    property alias editChange: editChange
    anchors.fill: parent
    Text {
        id: heading
        objectName: "heading"
        text: recipeName
        font.pointSize: fontMetrics.height * 1.3
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }
    SwipeView {
        id: swiper
        currentIndex: 0
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.top: heading.bottom
        anchors.topMargin: fontMetrics.maximumCharacterWidth * .2
        clip: true
        interactive: false
        Item {
            Button {
                id: editChange
                objectName: "editChange"
                text: "Change recipes"
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                height: fontMetrics.height * 2
            }
            Item {
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.top: editChange.bottom
                anchors.topMargin: fontMetrics.height * .5
                BetterColumn {
                    column {
                        id: recipeColumns
                        objectName: "recipeColumns"
                        spacing: fontMetrics.height
                    }
                }
            }
        }

        Item {
            id: element1
            ComboBox {
                id: recipeSelector
                objectName: "recipeSelector"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                model: recipeValues
                height: fontMetrics.height * 2
                signal recipeSelect(index: int)
            }
            Item {
                anchors.topMargin: fontMetrics.maximumCharacterWidth * .2
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: spinBox.top
                anchors.top: recipeSelector.bottom
                BetterColumn {
                    column {
                        id: recipeMaterials
                        objectName: "recipeMaterials"
                    }
                }
            }
            SpinBox {
                id: spinBox
                objectName: "spinBox"
                anchors.bottom: baseRow.top
                anchors.bottomMargin: fontMetrics.height * .5
                anchors.horizontalCenter: parent.horizontalCenter
                editable: true
                to: 999999
                enabled: !isDefaultRecipe
                Keys.onReturnPressed: accept_change();
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
                    id: cancel
                    text: qsTr("Cancel")
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Keys.onReturnPressed: cancel_change();
                }
                Button {
                    id: accept
                    text: qsTr("Accept")
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Keys.onReturnPressed: accept_change();
                }
            }
        }
    }
}
