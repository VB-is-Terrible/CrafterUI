import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import com.chow 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Testing Layout")
    color: 'orange'

    TestLayout {
        objectName: "Centre of the Universe"
    }
}

/*##^##
Designer {
    D{i:2;invisible:true}
}
##^##*/
