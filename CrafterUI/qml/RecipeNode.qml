import QtQuick 2.9

RecipeNodeForm {
    mouseArea{
        onClicked: {
            clicked(this.heading)
        }
    }
    signal clicked(name: string)

}
