import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQuick.Window 2.12
import myextension 1.0

Window {
    height: 700
    width: 250
    color: "black"
    flags: Qt.FramelessWindowHint

    GroupBox {
        title: qsTr("Settings")
        anchors.fill: parent
        anchors.margins: 10

        Column {
            id: column
            anchors.fill: parent

            Switch {
                id: fastRenderSetting
                text: qsTr("Fast Render")
                checked: settings.fastRenderEnabled

                onClicked: {
                    settings.fastRenderEnabled = checked
                }
            }

            Switch {
                id: setting2
                text: qsTr("Setting 2")
            }

            Switch {
                id: setting3
                text: qsTr("Setting 3")
            }
        }
    }
}


