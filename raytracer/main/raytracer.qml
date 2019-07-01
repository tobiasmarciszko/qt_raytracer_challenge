import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import myextension 1.0

Window {

    id: window
    visible: true
    width: 640
    height: 480
    color: "#3f5d80"
    title: "Freeside Raytracer"
    // x: Screen.width / 2 - width / 2
    // y: Screen.height / 2 - height / 2

    Rectangle {
        id: rectangle
        color: "white"
        radius: 0
        anchors.right: parent.right
        anchors.rightMargin: 160
        anchors.left: parent.left
        anchors.leftMargin: 160
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 120
        anchors.top: parent.top
        anchors.topMargin: 120
        border.color: "#00000000"
        border.width: 0
        clip: true

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            property var initialPositionX: 0
            property var initialPositionY: 0

            onPressed: {
                initialPositionX = mouse.x
                initialPositionY = mouse.y
            }

            onPositionChanged: {
                let deltaX = (initialPositionX - mouse.x) / 500;
                let deltaY = (initialPositionY - mouse.y) / 500;

                raytracer.fromXYChanged(deltaX, deltaY)
            }

            ImageItem {
                id: liveImageItem
                fillColor: "#FFFFFF"
                clip: true
                enabled: false
                smooth: false
                antialiasing: false
                anchors.fill: parent

                Component.onCompleted: {
                    raytracer.render(rectangle.width, rectangle.height)
                }

                Connections {
                    target: raytracer
                    onRendererReady: liveImageItem.setImage(image)
                }
            }

            BusyIndicator {
                id: busyIndicator
                x: 130
                y: 90
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
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
        }
    }

    Slider {
        id: slider
        y: 371
        height: 40
        anchors.left: parent.left
        anchors.leftMargin: 147
        anchors.right: parent.right
        anchors.rightMargin: 147
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 69
        stepSize: 0.1
        to: 10
        from: -10
        value: 0

        onMoved: {
            raytracer.fromXChanged(slider.value)
        }
    }

    Slider {
        id: slider1
        x: 486
        width: 57
        anchors.top: parent.top
        anchors.topMargin: 120
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 120
        anchors.right: parent.right
        anchors.rightMargin: 97
        orientation: Qt.Vertical
        stepSize: 0.1
        to: 10
        value: 1.5
        from: -10

        onMoved: {
            raytracer.fromYChanged(slider1.value)
        }
    }

    Text {
        id: element1
        y: 447
        text: qsTr("Phong")
        anchors.left: parent.left
        anchors.leftMargin: 7
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        font.pointSize: 11
    }

    Switch {
        id: element
        y: 436
        anchors.left: parent.left
        anchors.leftMargin: 51
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        display: AbstractButton.IconOnly

        onCheckedChanged: {
            raytracer.switchChanged()
        }
    }

    Text {
        id: element2
        y: 447
        text: qsTr("Blinn-Phong")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 131
        font.pointSize: 11
    }

    RoundButton {
        id: button
        x: 270
        y: 424
        width: 80
        text: qsTr("Render")
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        antialiasing: true
        enabled: !raytracer.rendering
        onClicked: {
            raytracer.render(rectangle.width, rectangle.height)
        }
    }

    Text {
        id: element3
        x: 295
        text: rectangle.width + "x" + rectangle.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 99
        font.pixelSize: 12
    }
}





































/*##^## Designer {
    D{i:6;anchors_x:376}D{i:5;anchors_x:130;anchors_y:90}D{i:13;anchors_y:99}
}
 ##^##*/
