import QtQuick 2.4

RecipeDetailedForm {
    recipeSelector {
        onCurrentIndexChanged: {
            this.recipeSelector.recipeSelect(this.recipeSelector.currentIndex);
        }
    }
    cancel {
        onClicked: cancel_change();
    }
    editChange {
        onClicked: {
            swiper.setCurrentIndex(1);
        }
    }
    accept {
        onClicked: accept_change();
    }

    function accept_change() {
        swiper.setCurrentIndex(0);
        changeRecipe(spinBox.value);
    }

    function cancel_change() {
        swiper.setCurrentIndex(0);
        spinBox.value = this.recipeCount;
    }

    function toOverview () {
        swiper.setCurrentIndex(0);
    }

    signal changeRecipe(new_amount: int);
}
