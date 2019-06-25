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
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2

    Rectangle {
        id: rectangle
        x: 134
        y: 78
        width: 320
        height: 240
        color: "#00000000"
        radius: 0
        border.color: "#00000000"
        border.width: 0
        clip: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

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
                x: 0
                y: 0
                fillColor: "#000000"
                clip: true
                enabled: false
                smooth: false
                antialiasing: false
                anchors.fill: parent

                Component.onCompleted: {
                    raytracer.update()
                }

                Connections {
                    target: raytracer
                    onRendererReady: liveImageItem.setImage(image)
                }
            }
        }
    }

    Slider {
        id: slider
        x: 147
        y: 371
        width: 347
        height: 40
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
        y: 120
        width: 57
        height: 240
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
        x: 7
        y: 447
        text: qsTr("Phong")
        font.pointSize: 11
        anchors.right: element.left
        anchors.rightMargin: 0
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: element.verticalCenter
    }

    Switch {
        id: element
        x: 51
        y: 436
        anchors.verticalCenter: element1.verticalCenter
        display: AbstractButton.IconOnly

        onCheckedChanged: {
            raytracer.switchChanged()
        }
    }

    Text {
        id: element2
        x: 131
        y: 447
        text: qsTr("Blinn-Phong")
        anchors.left: element.right
        anchors.leftMargin: 0
        anchors.right: element.left
        anchors.verticalCenter: element.verticalCenter
        font.pointSize: 11
        anchors.rightMargin: 6
        anchors.verticalCenterOffset: 0
    }

}













/*##^## Designer {
    D{i:2;anchors_height:240;anchors_width:320;anchors_x:0;anchors_y:0}
}
 ##^##*/
