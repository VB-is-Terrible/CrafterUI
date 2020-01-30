import QtQuick 2.9

SideColumnForm {
    button1 {
        onClicked: {
            mainView.setCurrentIndex(0);
        }
    }
    function toRecipes() {
        mainView.setCurrentIndex(1);
    }
    function toRawMaterials () {
        mainView.setCurrentIndex(0);
    }
}
