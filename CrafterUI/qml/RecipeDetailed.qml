import QtQuick 2.4

RecipeDetailedForm {
    recipeSelector {
        onCurrentIndexChanged: {
            this.recipeSelector.recipeSelect(this.recipeSelector.currentIndex);
        }
    }
    cancel {
        onClicked: {
            swiper.setCurrentIndex(0);
            spinBox.value = this.recipeCount;
        }
    }
    editChange {
        onClicked: {
            swiper.setCurrentIndex(1);
        }
    }
    accept {
        onClicked: {
            swiper.setCurrentIndex(0);
            changeRecipe(spinBox.value);
        }
    }
    function toOverview () {
        swiper.setCurrentIndex(0);
    }

    signal changeRecipe(new_amount: int);
}
