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

            Component.onCompleted: {
                if (rectangle.width > 0 && rectangle.height > 0) {
                    raytracer.setViewportSize(rectangle.width, rectangle.height)
                    raytracer.render()
                }
            }

            Connections {
                target: raytracer
                onImageReady: liveImageItem.setImage(image)
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            onClicked: {
                print("Object selected: " + raytracer.objectIdFromCoordinates(mouseX, mouseY))
            }
        }
    }

    DropShadow {
        anchors.fill: rectangle
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: rectangle
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
            raytracer.fromY = slider1.value
            raytracer.wireframe()
        }
    }

    Switch {
        id: element
        y: 430
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        display: AbstractButton.IconOnly

        onCheckedChanged: {
            raytracer.switchChanged()
        }
    }

    Text {
        id: element2
        text: qsTr("Blinn-Phong")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
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

}







































































































/*##^## Designer {
    D{i:6;anchors_height:100;anchors_width:100}
}
 ##^##*/
