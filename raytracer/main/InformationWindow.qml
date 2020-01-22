import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.2
import myextension 1.0

Item {
    id: informationWindow
    height: 700
    width: 300

    property alias moveButton: moveButton
    property alias scaleButton: scaleButton
    property alias rotXButton: rotateXButton
    property alias rotYButton: rotateYButton
    property alias rotZButton: rotateZButton

    property alias colorDialog: colorDialog
    property alias ambientSlider: ambientSlider
    property alias diffuseSlider: diffuseSlider
    property alias specularSlider: specularSlider

    GroupBox {
        id: groupBox1
        title: qsTr("Object Manipulation")
        anchors.fill: parent
        anchors.margins: 10

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
                    rotateXButton.checked = false
                    rotateYButton.checked = false
                    rotateZButton.checked = false
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
                    rotateXButton.checked = false
                    rotateYButton.checked = false
                    rotateZButton.checked = false
                }
            }
        }

        Button {
            id: colorButton
            x: 140
            y: -8
            text: qsTr("Color")
            anchors.topMargin: 0
            checkable: false
            anchors.top: parent.top

            onClicked: {
                colorDialog.color = raytracer.selectedObject.color
                colorDialog.open()
            }
        }

        ColorDialog {
            id: colorDialog
            title: "Please choose a color"

            onCurrentColorChanged: {
                raytracer.selectedObject.color = colorDialog.currentColor
                raytracer.materialPreview()
                raytracer.wireframe()
            }
        }

        Column {
            anchors.bottom: rectangle.top
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: scaleButton.bottom
            anchors.topMargin: 64

            Slider {
                id: ambientSlider
                x: 2
                y: 130
                width: 230
                height: 48
                stepSize: 0.01
                value: raytracer.selectedObject.ambient

                Label {
                    text: "ambient"
                    anchors.left: parent.left
                }

                Label {
                    text: parent.value.toFixed(2)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                onMoved: {
                    raytracer.selectedObject.ambient = value
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
                value: raytracer.selectedObject.diffuse

                Label {
                    text: "diffuse"
                    anchors.left: parent.left
                }

                Label {
                    text: parent.value.toFixed(2)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                onMoved: {
                    raytracer.selectedObject.diffuse = value
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
                value: raytracer.selectedObject.specular

                Label {
                    text: "specular"
                    anchors.left: parent.left
                }

                Label {
                    text: parent.value.toFixed(2)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                onMoved: {
                    raytracer.selectedObject.specular = value
                    raytracer.materialPreview()
                }
            }

            Slider {
                id: reflectiveSlider
                x: 2
                y: 250
                width: 230
                height: 48
                stepSize: 0.01
                value: raytracer.selectedObject.reflective
                Label {
                    text: "reflective"
                    anchors.left: parent.left
                }

                Label {
                    text: parent.value.toFixed(2)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                onMoved: {
                    raytracer.selectedObject.reflective = value
                    raytracer.materialPreview()
                }
            }

            Slider {
                id: transparencySlider
                x: 2
                y: 290
                width: 230
                height: 48
                stepSize: 0.01
                value: raytracer.selectedObject.transparency
                Label {
                    text: "transp."
                    anchors.left: parent.left
                }

                Label {
                    text: parent.value.toFixed(2)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                onMoved: {
                    raytracer.selectedObject.transparency = value
                    raytracer.materialPreview()
                }
            }

            Slider {
                id: rIndexSlider
                x: 2
                y: 330
                width: 230
                height: 48
                to: 5
                stepSize: 0.01
                value: raytracer.selectedObject.rindex
                Label {
                    text: "r-index"
                    anchors.left: parent.left
                }

                Label {
                    text: parent.value.toFixed(2)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                onMoved: {
                    raytracer.selectedObject.rindex = value
                    raytracer.materialPreview()
                }
            }

            Slider {
                id: shininessSlider
                x: 2
                y: 370
                width: 230
                height: 48
                from: 0
                to: 500
                stepSize: 1
                value: raytracer.selectedObject.shininess
                Label {
                    text: "shininess"
                    anchors.left: parent.left
                }

                Label {
                    text: parent.value.toFixed(2)
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                onMoved: {
                    raytracer.selectedObject.shininess = value
                    raytracer.materialPreview()
                }
            }
        }

        Rectangle {
            id: rectangle
            x: 58
            y: 538
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

        Button {
            id: rotateXButton
            x: 0
            y: -8
            text: qsTr("Rot X")
            anchors.topMargin: 49
            anchors.top: parent.top
            checkable: true

            onCheckedChanged: {
                if (checked) {
                    moveButton.checked = false
                    scaleButton.checked = false
                    rotateYButton.checked = false
                    rotateZButton.checked = false
                }
            }
        }

        Button {
            id: rotateYButton
            x: 70
            y: 0
            text: qsTr("Rot Y")
            anchors.topMargin: 49
            anchors.top: parent.top
            checkable: true

            onCheckedChanged: {
                if (checked) {
                    moveButton.checked = false
                    scaleButton.checked = false
                    rotateXButton.checked = false
                    rotateZButton.checked = false
                }
            }
        }

        Button {
            id: rotateZButton
            x: 140
            y: -6
            text: qsTr("Rot Z")
            anchors.topMargin: 49
            anchors.top: parent.top
            checkable: true

            onCheckedChanged: {
                if (checked) {
                    moveButton.checked = false
                    scaleButton.checked = false
                    rotateXButton.checked = false
                    rotateYButton.checked = false
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:2;anchors_y:30}D{i:3;anchors_y:18}D{i:4;anchors_y:18}D{i:5;anchors_y:18}D{i:6;anchors_height:462}
D{i:31;anchors_y:30}D{i:32;anchors_y:30}D{i:33;anchors_y:30}
}
##^##*/
