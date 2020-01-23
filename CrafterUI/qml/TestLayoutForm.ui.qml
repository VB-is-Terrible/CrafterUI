import QtQuick 2.4
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.3

Item {

    anchors.fill: parent
    anchors.centerIn: parent
    property alias mainGraph: mainGraph
    property alias sideColumn: sideColumn


    FontMetrics {
        id: fontMetrics
    }

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        MainGraph {
            id: mainGraph
            transformOrigin: Item.Center
        }

        SideColumn {
            id: sideColumn
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_height:100;anchors_width:100;anchors_x:126;anchors_y:107}
D{i:2;anchors_height:400;anchors_width:200;anchors_x:155;anchors_y:66}
}
##^##*/
