import QtQuick 2.13
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2
import QtQuick.Shapes 1.13
import com.chow 1.0

Item {
    id: page

    property alias backArea: backArea
    property alias scene: flickable
    property alias vertibar: vertibar
    FontMetrics {
        id: fontMetrics
    }

    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.minimumWidth: 400 + fontMetrics.maximumCharacterWidth * 2
    Layout.minimumHeight: 300
    Layout.preferredHeight: 600
    Layout.preferredWidth: 60 * fontMetrics.maximumCharacterWidth

    height: 480
    property alias flickable: flickable
    width: 600

    Rectangle {
        anchors.fill: parent
        color: 'teal'
        Flickable {
            id: flickable
            interactive: true

            anchors.fill: parent
            bottomMargin: fontMetrics.maximumCharacterWidth
            topMargin: fontMetrics.maximumCharacterWidth
            rightMargin: fontMetrics.maximumCharacterWidth
            leftMargin: fontMetrics.maximumCharacterWidth
            contentHeight: 1080
            contentWidth: 1920
            clip: true

            ScrollBar.vertical: ScrollBar {
                size: 20
                id: vertibar
                x: parent.right - size
                policy: ScrollBar.AlwaysOn
            }
            ScrollBar.horizontal: ScrollBar {
                size: 20
                y: parent.bottom - size
                policy: ScrollBar.AlwaysOn
            }

            MouseArea {
                id: backArea
                anchors.fill: parent
                //                propagateComposedEvents: true
                z: -99
            }

            Label {
                text: qsTr("You are on Main Graph.")
                anchors.centerIn: parent
            }
            Item {
                id: recipeStore
                anchors.fill: parent
                objectName: "recipeStore"
                RecipeNode {
                    id: test
                    x: 280
                    y: 8
                }
                RecipeNode {
                    x: 515
                    y: 378
                }

                RecipeNode {
                    x: 46
                    y: 378
                }
            }

            LineShape {
                anchors.fill: parent
                objectName: "betterShape"
                id: betterShape
                z: -100
                lineWidth: 4
            }

            //            Shape {

            //                id: shape
            //                objectName: "shape"
            //                width: 1920
            //                height: 1080
            //                z: -100
            //                anchors.left: parent.left
            //                anchors.leftMargin: 0
            //                anchors.top: parent.top
            //                anchors.topMargin: 0
            //                LinePath {
            //                    startX: 480
            //                    startY: 208
            //                    PathLine {
            //                        x: 246
            //                        y: 378
            //                    }
            //                }
            //                LinePath {
            //                    startX: 480
            //                    startY: 208
            //                    PathLine {
            //                        x: 715
            //                        y: 378
            //                    }
            //                }
            //            }
        }
    }
}

/*##^##
Designer {
    D{i:4;anchors_height:400;anchors_width:400}
}
##^##*/

