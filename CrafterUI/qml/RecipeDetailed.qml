import QtQuick 2.4

RecipeDetailedForm {
    recipeSelector {
        onCurrentIndexChanged: {
            console.log(this.recipeSelector.currentIndex);
            this.recipeSelector.recipeSelect(this.recipeSelector.currentIndex);
        }
    }

}
