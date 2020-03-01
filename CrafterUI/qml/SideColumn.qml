import QtQuick 2.9

SideColumnForm {
    button1 {
        onClicked: {
            toRawMaterials();
        }
    }
    selectInput {
        onClicked: {
            toRecipeInput();
        }
    }

    function toRecipes() {
        mainView.setCurrentIndex(2);
    }
    function toRawMaterials () {
        mainView.setCurrentIndex(1);
    }
    function toRecipeInput () {
        mainView.setCurrentIndex(0);
    }
}
