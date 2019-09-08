import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import myextension 1.0
import QtGraphicalEffects 1.0

Window {

    id: window
    visible: true
    width: 747
    height: 602
    color: "#3f5d80"
    title: "Freeside Raytracer"

    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2

        raytracer.setViewportSize(640, 480)
        raytracer.wireframe()
    }

    BusyIndicator {
        id: busyIndicator
        x: 674
        y: 528
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 14
        anchors.right: parent.right
        anchors.rightMargin: 13
        running: raytracer.rendering

        Text {
            id: element4
            color: "#ffffff"
            visible: raytracer.rendering
            text: raytracer.progress + "%"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
        }
    }

    Rectangle {
        id: rectangle
        color: "white"
        anchors.right: parent.right
        anchors.rightMargin: 87
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 102
        anchors.top: parent.top
        anchors.topMargin: 20
        clip: true
        ImageItem {
            id: liveImageItem
            fillColor: "#FFFFFF"
            clip: true
            enabled: false
            anchors.fill: parent

            Connections {
                target: raytracer
                onImageReady: liveImageItem.setImage(image)
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            onClicked: {
                selectedObject = raytracer.objectFromCoordinates(mouseX, mouseY)
                updateTextbox()
            }
        }

        ColorOverlay {
                anchors.fill: liveImageItem
                source: liveImageItem
                color: "#80800000"
                visible: raytracer.rendering
            }
    }

    DropShadow {
        id: shadow
        anchors.fill: rectangle
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: rectangle
        visible: !raytracer.rendering
    }

    Slider {
        id: slider
        y: 366
        height: 40
        width: rectangle.width
        anchors.horizontalCenter: rectangle.horizontalCenter
        anchors.top: rectangle.bottom

        stepSize: 0.1
        to: 10
        from: -10
        value: raytracer.fromX

        onMoved: {
            updateTextbox()
            raytracer.fromX = slider.value
            raytracer.wireframe()
        }
    }

    Slider {
        id: slider1
        width: 57
        height: rectangle.height
        anchors.verticalCenter: rectangle.verticalCenter
        anchors.left: rectangle.right


        orientation: Qt.Vertical
        stepSize: 0.1
        to: 10
        value: raytracer.fromY
        from: -10

        onMoved: {
            updateTextbox()
            raytracer.fromY = slider1.value
            raytracer.wireframe()
        }
    }

    Switch {
        id: element
        y: 552
        anchors.left: parent.left
        anchors.leftMargin: 62
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        display: AbstractButton.IconOnly

        onCheckedChanged: {
            raytracer.switchChanged()
        }
    }

    Text {
        id: element2
        y: 564
        text: qsTr("Blinn-Phong")
        anchors.leftMargin: 6
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 21
        anchors.left: element.right
        font.pointSize: 11
    }

    RoundButton {
        id: button
        x: 400
        y: 420
        width: 80
        text: qsTr("Render")
        radius: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: rectangle.right

        antialiasing: true
        enabled: !raytracer.rendering
        onClicked: {
            raytracer.setViewportSize(rectangle.width, rectangle.height)
            raytracer.render()
        }
    }

    Text {
        id: element3
        text: rectangle.width + "x" + rectangle.height
        anchors.horizontalCenter: rectangle.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        font.pixelSize: 14
    }

    Text {
        id: element5
        text: slider.value.toFixed(1)
        anchors.top: slider.bottom
        anchors.horizontalCenter: slider.horizontalCenter
        font.pixelSize: 12
    }

    Text {
        id: element6
        text: slider1.value.toFixed(1)
        anchors.left: slider1.right
        anchors.verticalCenter: slider1.verticalCenter
        font.pixelSize: 12
    }

    RoundButton {
        id: wireframeButton
        x: 480
        y: 542
        width: 94
        height: 40
        radius: 8
        text: qsTr("Wireframe")
        anchors.right: parent.right
        anchors.rightMargin: 173
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        antialiasing: true
        enabled: !raytracer.rendering

        onClicked: {
            raytracer.setViewportSize(rectangle.width, rectangle.height)
            raytracer.wireframe()
        }
    }

    Text {
        id: element7
        x: -9
        y: -8
        text: "Last frame rendered in " + raytracer.lastRenderTime + "ms"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.horizontalCenter: rectangle.horizontalCenter
        font.pixelSize: 14
    }

    ProgressBar {
        id: progressBar
        x: -35
        y: 49
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: wireframeButton.left
        anchors.right: button.right
        to: 100
        value: raytracer.progress
        visible: raytracer.rendering
    }

    Window {

        id: window2
        title: "Information"
        width: 250
        height: window.height
        x: window.x-250
        y: window.y

        TextArea {
            id: informationBox
            anchors.fill: parent
            wrapMode: TextEdit.Wrap
        }
    }

    RoundButton {
        id: informationButton
        y: 552
        text: "i"
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        onPressed: {
            if (window2.visibility === Window.Hidden) {
                updateTextbox()
                window2.show()
            } else if (window2.visibility !== Window.Hidden) {
                window2.close()
            }
        }
    }

    property ShapeBridge selectedObject
    function updateTextbox() {

        informationBox.x = window.x-250;
        informationBox.y = window.y;

        informationBox.text = "Camera Position: \n"
        informationBox.text += "x: " + raytracer.fromX.toFixed(1) + "\n"
        informationBox.text += "y: " + raytracer.fromY.toFixed(1) + "\n"
        informationBox.text += "z: " + raytracer.fromZ.toFixed(1) + "\n\n"

        if (selectedObject != null) {

            informationBox.text += "Object Selection\n\n"
            informationBox.text += "Color: " + selectedObject.color + "\n"
            informationBox.text += "Transform: \n"
            informationBox.text += selectedObject.transform.row(0) + "\n"
            informationBox.text += selectedObject.transform.row(1) + "\n"
            informationBox.text += selectedObject.transform.row(2) + "\n"
            informationBox.text += selectedObject.transform.row(3) + "\n"
        }
    }
}






























































































































































/*##^##
Designer {
    D{i:6;anchors_height:100;anchors_width:100}D{i:22;anchors_x:16}
}
##^##*/
