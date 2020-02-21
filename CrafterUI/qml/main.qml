import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import com.chow 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: qsTr("Crafting helper")
    color: 'orange'

    TestLayout {
        objectName: "Centre of the Universe"
    }
}
