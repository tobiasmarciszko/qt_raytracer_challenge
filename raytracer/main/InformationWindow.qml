import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.2
import myextension 1.0

ApplicationWindow {
    id: informationWindow
    title: "Information"
    height: 700
    width: 300
    color: "black"
    flags: Qt.FramelessWindowHint

    property var selectedObject

    property alias moveButton: moveButton
    property alias scaleButton: scaleButton
    property alias colorDialog: colorDialog
    property alias ambientSlider: ambientSlider
    property alias diffuseSlider: diffuseSlider
    property alias specularSlider: specularSlider

    GroupBox {
        id: groupBox1
        title: qsTr("Information")
        anchors.fill: parent
        anchors.margins: 10

        GroupBox {
            id: groupBox
            x: 0
            width: 256
            height: 336
            anchors.top: parent.top
            anchors.topMargin: 15
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

            Button {
                id: colorButton
                x: 0
                y: -8
                text: qsTr("Color")
                anchors.topMargin: 54
                checkable: false
                anchors.top: parent.top

                onClicked: {
                    colorDialog.color = selectedObject.color
                    colorDialog.open()
                }
            }

            ColorDialog {
                id: colorDialog
                title: "Please choose a color"

                onCurrentColorChanged: {
                    selectedObject.color = colorDialog.currentColor
                    raytracer.materialPreview()
                }
            }

            Slider {
                id: ambientSlider
                x: 2
                y: 130
                width: 230
                height: 48
                stepSize: 0.01
                value: 0.5

                Label {
                    text: "ambient"
                    anchors.left: parent.left
                }

                 Label {
                     text: parent.value.toFixed(2)
                     anchors.horizontalCenter: parent.horizontalCenter
                 }

                 onMoved: {
                     informationWindow.selectedObject.ambient = value
                     raytracer.materialPreview()
                 }
            }

            Slider {
                id: diffuseSlider
                x: 2
                y: 170
                width: 230
                height: 48
                stepSize: 0.01
                value: 0.5

                Label {
                    text: "diffuse"
                    anchors.left: parent.left
                }

                 Label {
                     text: parent.value.toFixed(2)
                     anchors.horizontalCenter: parent.horizontalCenter
                 }

                 onMoved: {
                     informationWindow.selectedObject.diffuse = value
                     raytracer.materialPreview()
                 }
            }

            Slider {
                id: specularSlider
                x: 2
                y: 210
                width: 230
                height: 48
                stepSize: 0.01
                value: 0.5

                Label {
                    text: "specular"
                    anchors.left: parent.left
                }

                 Label {
                     text: parent.value.toFixed(2)
                     anchors.horizontalCenter: parent.horizontalCenter
                 }

                 onMoved: {
                     informationWindow.selectedObject.diffuse = value
                     raytracer.materialPreview()
                 }
            }
        }

        Rectangle {
            id: rectangle
            y: 398
            width: 140
            height: 140
            color: "#ffffff"
            border.color: "#ff2222"
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
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
    D{i:3;anchors_y:18}D{i:4;anchors_y:18}D{i:5;anchors_y:18}D{i:2;anchors_y:30}
}
##^##*/
