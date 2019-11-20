import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import myextension 1.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.12
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0 as Core
import Qt3D.Render 2.0 as Render
import Qt3D.Input 2.0 as Input
import Qt3D.Extras 2.0 as Extras

Window {

    id: window
    visible: true
    width: 747
    height: 602
    color: "black"
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

        Label {
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
        Material.background: "red"
        anchors.right: parent.right
        anchors.rightMargin: 87
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 102
        anchors.top: parent.top
        anchors.topMargin: 20
        clip: false

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

        Scene3D {
            id: scene3d
            anchors.fill: parent
            x: liveImageItem.x
            y: liveImageItem.y
            width: liveImageItem.width
            height: liveImageItem.height
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.UserAspectRatio

        Core.Entity {
            id: sceneRoot

            Render.Camera {
                id: camera
                projectionType: Render.CameraLens.CustomProjection
                fieldOfView: 45
                nearPlane : 0.1
                farPlane : 1000.0
                property var scaleX: 1 / Math.tan(fieldOfView * 0.5 * (Math.PI/180))
                property var scaleY: 1 / Math.tan(fieldOfView * 0.5 * (Math.PI/180))
                position: Qt.vector3d(raytracer.fromX, raytracer.fromY, raytracer.fromZ)
                upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                viewCenter: Qt.vector3d(0, 0, 0)
                projectionMatrix: Qt.matrix4x4(
                                      scaleX, 0, 0, 0,
                                      0, scaleY, 0, 0,
                                      0, 0, -1, -1,
                                      0, 0, -1, 0
                                      )
            }

            // Extras.FirstPersonCameraController { camera: camera }

            components: [
                Render.RenderSettings {
                    activeFrameGraph: Extras.ForwardRenderer {
                        camera: camera
                        clearColor: "transparent"
                    }
                }
                ,
                Input.InputSettings { }
            ]

            Extras.PhongMaterial {
                id: material
            }

            Extras.GoochMaterial {
                id: gooch
            }

//            Extras.TorusMesh {
//                id: torusMesh
//                radius: 3
//                minorRadius: 1
//                rings: 100
//                slices: 20
//            }

//            Core.Transform {
//                id: torusTransform
//                scale3D: Qt.vector3d(1, 1, 1)
//                rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
//                translation: Qt.vector3d(0, 0, 0);
//            }

//            Core.Entity {
//                id: torusEntity
//                components: [ torusMesh, material, torusTransform ]
//            }

            Extras.SphereMesh {
                id: middle
                radius: 1
            }

            Core.Transform {
                id: sphereTransform
                translation: Qt.vector3d(0, 0, 0);
            }

            Core.Entity {
                id: sphereEntity
                components: [ middle, gooch, sphereTransform ]
            }

            Extras.SphereMesh {
                id: right
                radius: 1
            }

            Core.Transform {
                id: rightTransform
                translation: Qt.vector3d(3, 0, 0);
            }

            Core.Entity {
                id: rightEntity
                components: [ right, material, rightTransform ]
            }

            Extras.SphereMesh {
                id: left
                radius: 1
            }

            Core.Transform {
                id: leftTransform
                property real userAngle: 0.0
                translation: Qt.vector3d(-3, 0, 0);

            }

            Core.Entity {
                id: leftEntity
                components: [ left, material, leftTransform ]
            }


        }
    }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: raytracer.selectObject(mouseX, mouseY)

            onWheel: {

                raytracer.fromZ += wheel.angleDelta.y * 1/8 / 50;
                console.log(raytracer.fromZ);
                raytracer.wireframe()
            }

            Connections {
                target: raytracer
                onObjectSelected: {
                    selectedObject = shapeBridge
                    raytracer.materialPreview()
                    updateTextbox()
                }
            }
        }

        ColorOverlay {
                anchors.fill: liveImageItem
                source: liveImageItem
                color: "#80800000"
                visible: raytracer.rendering
            }
    }

//    DropShadow {
//        id: shadow
//        anchors.fill: rectangle
//        horizontalOffset: 3
//        verticalOffset: 3
//        radius: 8.0
//        samples: 17
//        color: "#8FFFFFFFF"
//        source: rectangle
//        visible: !raytracer.rendering
//    }

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
            scene3d.visible = !scene3d.visible
        }
    }

    Label {
        id: element2
        y: 564
        color: "teal"
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
        width: 110
        text: qsTr("Render")
        display: AbstractButton.TextOnly
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

    Label {
        id: element3
        text: rectangle.width + "x" + rectangle.height
        anchors.horizontalCenter: rectangle.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        font.pixelSize: 14
    }

    Label {
        id: element5
        text: slider.value.toFixed(1)
        anchors.top: slider.bottom
        anchors.horizontalCenter: slider.horizontalCenter
        font.pixelSize: 12
    }

    Label {
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
        width: 110
        radius: 8
        text: qsTr("Wireframe")
        display: AbstractButton.TextOnly
        anchors.right: button.left
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        antialiasing: true
        enabled: !raytracer.rendering

        onClicked: {
            raytracer.setViewportSize(rectangle.width, rectangle.height)
            raytracer.wireframe()
        }
    }

    Label {
        id: element7
        x: -9
        y: -8
        text: "Last frame rendered in " + raytracer.lastRenderTime + "ms"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.horizontalCenter: rectangle.horizontalCenter
        font.pixelSize: 14
        visible: !raytracer.rendering
    }

    ProgressBar {
        id: progressBar
        anchors.horizontalCenter: element3.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        to: 100
        value: raytracer.progress
        visible: raytracer.rendering
    }

    InformationWindow {
        id: informationWindow
        y: window.y
        x: window.x-250
        height: window.height
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
            if (informationWindow.visibility === Window.Hidden) {
                updateTextbox()
                informationWindow.show()
            } else if (informationWindow.visibility !== Window.Hidden) {
                informationWindow.close()
            }
        }
    }

    property ShapeBridge selectedObject
    function updateTextbox() {

        informationWindow.text = "Camera Position: \n"
        informationWindow.text += "x: " + raytracer.fromX.toFixed(1) + "\n"
        informationWindow.text += "y: " + raytracer.fromY.toFixed(1) + "\n"
        informationWindow.text += "z: " + raytracer.fromZ.toFixed(1) + "\n\n"

        if (selectedObject !== null) {

            informationWindow.text += "Object Selection\n\n"
            informationWindow.text += "Color: " + selectedObject.color + "\n"
            informationWindow.text += "Transform: \n"
            informationWindow.text += selectedObject.transform.row(0) + "\n"
            informationWindow.text += selectedObject.transform.row(1) + "\n"
            informationWindow.text += selectedObject.transform.row(2) + "\n"
            informationWindow.text += selectedObject.transform.row(3) + "\n"
        }
    }
}































































































































































