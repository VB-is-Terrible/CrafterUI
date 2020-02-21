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
        spacing: 0
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
