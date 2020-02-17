import QtQuick 2.9

MainGraphForm {
    backArea {
        onClicked: {
            backClicked();
        }
    }
    signal backClicked();
}
