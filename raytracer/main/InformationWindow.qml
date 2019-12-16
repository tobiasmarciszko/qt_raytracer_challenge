import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import myextension 1.0

ApplicationWindow {
    title: "Information"
    height: 700
    width: 300
    color: "black"
    flags: Qt.FramelessWindowHint

    property alias informationBox: informationBox
    property alias text: informationBox.text

    property alias moveButton: moveButton
    property alias scaleButton: scaleButton

    GroupBox {
        id: groupBox1
        title: qsTr("Information")
        anchors.fill: parent
        anchors.margins: 10

        GroupBox {
            id: groupBox
            x: 0
            width: 256
            height: 107
            anchors.top: parent.top
            anchors.topMargin: 0
            title: qsTr("Tools")

            Button {
                id: scaleButton
                x: 70
                text: qsTr("Scale")
                anchors.top: parent.top
                anchors.topMargin: 0
                checkable: true

                onCheckedChanged: {
                    if (checked) {
                        moveButton.checked = false
                    }
                }
            }

            Button {
                id: moveButton
                x: 0
                text: qsTr("Move")
                anchors.top: parent.top
                anchors.topMargin: 0
                checked: true
                checkable: true

                onCheckedChanged: {
                    if (checked) {
                        scaleButton.checked = false
                    }
                }
            }

        }

        TextArea {
            background: null
            id: informationBox
            text: ""
            anchors.topMargin: 113
            clip: true
            textFormat: Text.PlainText
            renderType: Text.QtRendering
            anchors.fill: parent
            wrapMode: TextEdit.NoWrap
            anchors.margins: 0
            cursorVisible: false
            readOnly: true
            font.pointSize: 10
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

/*##^##
Designer {
    D{i:3;anchors_y:18}D{i:4;anchors_y:18}D{i:2;anchors_y:30}
}
##^##*/
