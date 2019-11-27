import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import myextension 1.0

Window {
    title: "Information"
    height: 700
    width: 250
    color: "black"
    flags: Qt.FramelessWindowHint

    property alias informationBox: informationBox
    property alias text: informationBox.text
    GroupBox {
        title: qsTr("Information")
        anchors.fill: parent
        anchors.margins: 10

        TextArea {
            background: null
            id: informationBox
            text: ""
            clip: true
            textFormat: Text.PlainText
            renderType: Text.QtRendering
            anchors.fill: parent
            wrapMode: TextEdit.Wrap
            anchors.margins: 0
            cursorVisible: false
            readOnly: true
        }

        Rectangle {
            id: rectangle
            y: 314
            width: 140
            height: 140
            color: "#000000"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 26
            clip: true

            ImageItem {
                id: liveImageItem
                fillColor: "#FFFFFF"
                clip: true
                enabled: false
                anchors.fill: parent

                Connections {
                    target: raytracer
                    onMaterialPreviewReady: liveImageItem.setImage(image)
                }
            }
        }
    }
}
