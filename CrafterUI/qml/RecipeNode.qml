import QtQuick 2.9

RecipeNodeForm {
    mouseArea{
        onClicked: {
            console.log(heading)
            clicked(this.heading)
        }
    }
    signal clicked(name: string)

}
