import QtQuick 2.9
import QtQuick.Controls 2.9

Item {
    FontMetrics {
        id: fontMetrics
    }
    id: element
    property string recipeName: qsTr("Your recipe name here")
    property var recipeValues: ["The", "names", "of", "recipes", "here"]
    width: 400
    height: 400
    property alias recipeSelector: recipeSelector
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
        currentIndex: 0
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.top: heading.bottom
        anchors.topMargin: fontMetrics.maximumCharacterWidth * .2
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
                anchors.topMargin: fontMetrics.maximumCharacterWidth * .2
                id: recipeSelector
                objectName: "recipeSelector"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                model: recipeValues
                height: fontMetrics.height * 2
            }
            Item {
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.top: recipeSelector.bottom
                BetterColumn {
                    column {
                        id: recipeMaterials
                        objectName: "recipeMaterials"
                    }
                }
            }
        }
    }
}
