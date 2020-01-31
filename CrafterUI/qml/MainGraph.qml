import QtQuick 2.9

MainGraphForm {
    backArea {
//        onClicked: console.log("You've clicked on the background")
        onClicked: {
            backClicked();
        }
    }
    signal backClicked();
}
