import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

Item {
    TextArea {
        id: recipeInput
        placeholderText: "Recipe Input here"
        anchors.fill: parent
        objectName: "recipeInput"
    }
}
