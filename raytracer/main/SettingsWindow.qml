import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQuick.Window 2.12
import myextension 1.0

ApplicationWindow {
    height: 700
    width: 250
    color: "black"
    flags: Qt.FramelessWindowHint

    FontLoader {
        id: glyphs
        source: "fontello"
    }

    GroupBox {
        title: qsTr("Settings")
        anchors.fill: parent
        anchors.margins: 10

        Column {
            id: column
            anchors.fill: parent

            Row {
                spacing: 2
                Label {
                    font.family: glyphs.name
                    font.pointSize: 12
                    text: "\uf135"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    id: fastRenderSetting
                    text: qsTr("Fast Render")
                    checked: settings.fastRenderEnabled
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        settings.fastRenderEnabled = checked
                    }
                }
            }

            Row {
                spacing: 2

                Label {
                    font.family: glyphs.name
                    font.pointSize: 12
                    text: "\ue800"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    text: qsTr("Fireworks")
                    checked: settings.fireworksEnabled
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        settings.fireworksEnabled = checked
                    }
                }
            }

            Row {
                spacing: 2

                Label {
                    font.family: glyphs.name
                    font.pointSize: 12
                    text: "\ue800"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    text: qsTr("Setting")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Row {
                spacing: 2

                Label {
                    font.family: glyphs.name
                    font.pointSize: 12
                    text: "\ue800"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    text: qsTr("Setting")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Row {
                spacing: 2

                Label {
                    font.family: glyphs.name
                    font.pointSize: 12
                    text: "\ue800"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    text: qsTr("Setting")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Row {
                spacing: 2

                Label {
                    font.family: glyphs.name
                    font.pointSize: 12
                    text: "\ue800"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    text: qsTr("Setting")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Row {
                spacing: 2

                Label {
                    font.family: glyphs.name
                    font.pointSize: 12
                    text: "\ue800"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    text: qsTr("Setting")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}


