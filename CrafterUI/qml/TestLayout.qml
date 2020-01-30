import QtQuick 2.4

TestLayoutForm {
    mainGraph.backArea {
        onClicked: {
            sideColumn.toRawMaterials();
        }
    }
}
