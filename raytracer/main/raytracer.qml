import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import myextension 1.0
import QtQuick.Controls.Material 2.12
import QtQuick.Particles 2.12
import QtQuick.Layouts 1.13
import QtGraphicalEffects 1.0

ApplicationWindow {
    property alias glyphFont: glyphs.name

    font.family: aircraft.name
    id: window
    visible: true
    width: 747
    height: 602
    color: "black"
    title: "Freeside Raytracer"

    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
    }

    FontLoader {
        id: glyphs
        source: "fontello"
    }

    FontLoader {
        id: aircraft
        source: "B162"
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

    Pane {
        id: rectangle
        Material.background: "gray"
        anchors.right: parent.right
        anchors.rightMargin: 87
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 102
        anchors.top: parent.top
        anchors.topMargin: 20

        GridLayout {
            id: grid
            anchors.fill: parent
            columns: 2
            rows: 2
            columnSpacing: 10
            rowSpacing: 10

            Component.onCompleted: {
                raytracer.setViewportSize(liveImageItem4.width, liveImageItem4.height)
                raytracer.wireframe()
            }

            Connections {
                target: raytracer
                onWireframesReady: {
                    liveImageItem.setImage(top)
                    liveImageItem2.setImage(left)
                    liveImageItem3.setImage(right)
                    liveImageItem4.setImage(perspective)
                }
            }

            ImageItem {
                id: liveImageItem
                fillColor: "#FFFFFF"
                Layout.fillHeight: true
                Layout.fillWidth: true
                visible: !settings.fullscreenEnabled

                onWidthChanged: {
                    raytracer.setViewportSize(liveImageItem.width, liveImageItem.height)
                    raytracer.wireframe()
                }

                onHeightChanged: {
                    raytracer.setViewportSize(liveImageItem.width, liveImageItem.height)
                    raytracer.wireframe()
                }

                Text {
                    text: "Front"
                    anchors.left: parent.left
                    anchors.top: parent.top
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: raytracer.selectObject(mouseX, mouseY)

                    onWheel: {
                        raytracer.fromZ += wheel.angleDelta.y * 1/8 / 50;
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
            }

            ImageItem {
                id: liveImageItem2
                fillColor: "#FFFFFF"
                Layout.fillHeight: true
                Layout.fillWidth: true
                visible: !settings.fullscreenEnabled

                Text {
                    text: "Left"
                    anchors.left: parent.left
                    anchors.top: parent.top
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: raytracer.selectObject(mouseX, mouseY)

                    onWheel: {
                        raytracer.fromZ += wheel.angleDelta.y * 1/8 / 50;
                        raytracer.wireframe()
                    }
                }
            }

            ImageItem {
                id: liveImageItem3
                fillColor: "#FFFFFF"
                Layout.fillHeight: true
                Layout.fillWidth: true
                visible: !settings.fullscreenEnabled

                Text {
                    text: "Right"
                    anchors.left: parent.left
                    anchors.top: parent.top
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: raytracer.selectObject(mouseX, mouseY)

                    onWheel: {
                        raytracer.fromZ += wheel.angleDelta.y * 1/8 / 50;
                        raytracer.wireframe()
                    }
                }
            }

            ImageItem {
                id: liveImageItem4
                fillColor: "#FFFFFF"
                Layout.fillHeight: true
                Layout.fillWidth: true

                onWidthChanged: {
                    raytracer.setViewportSize(liveImageItem4.width, liveImageItem4.height)
                    raytracer.wireframe()
                }

                onHeightChanged: {
                    raytracer.setViewportSize(liveImageItem4.width, liveImageItem4.height)
                    raytracer.wireframe()
                }

                Connections {
                    target: raytracer
                    onImageReady: liveImageItem4.setImage(image)
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: raytracer.selectObject(mouseX, mouseY)

                    onWheel: {
                        raytracer.fromZ += wheel.angleDelta.y * 1/8 / 50;
                        raytracer.wireframe()
                    }
                }

                ColorOverlay {
                    anchors.fill: parent
                    source: parent
                    color: "#80800000"
                    visible: raytracer.rendering
                }
            }
        }
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

    RoundButton {
        id: button
        x: 400
        y: 420
        width: 110
        font.family: window.glyphFont
        font.pointSize: 18
        text: "\uf1b2"
        display: AbstractButton.TextOnly
        radius: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: rectangle.right

        antialiasing: true
        enabled: !raytracer.rendering
        onClicked: {
            raytracer.setViewportSize(liveImageItem4.width, liveImageItem4.height)
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
        font.family: glyphs.name
        font.pointSize: 18
        text: "\uf1cb"
        display: AbstractButton.TextOnly
        anchors.right: button.left
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        antialiasing: true
        enabled: !raytracer.rendering

        onClicked: {
            raytracer.setViewportSize(liveImageItem4.width, liveImageItem4.height)
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
        font.family: aircraft.name
    }

    RoundButton {
        id: informationButton
        y: 552
        font.family: window.glyphFont
        font.pointSize: 14
        text: "\ue801"
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        onPressed: {
            if (informationWindow.visibility === Window.Hidden) {
                updateTextbox()
                informationWindow.show()
                settingsWindow.close()
            } else if (informationWindow.visibility !== Window.Hidden) {
                informationWindow.close()
            }
        }
    }

    SettingsWindow {
        id: settingsWindow
        y: window.y
        x: window.x-250
        height: window.height
        font.family: aircraft.name
    }

    RoundButton {
        id: settingsButton
        y: 544
        font.family: window.glyphFont
        font.pointSize: 14
        text: "\ue800"
        anchors.left: informationButton.right
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        onPressed: {
            if (settingsWindow.visibility === Window.Hidden) {
                settingsWindow.show()
                informationWindow.close()
            } else if (settingsWindow.visibility !== Window.Hidden) {
                settingsWindow.close()
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

    ParticleSystem {
        id: particles
        anchors.fill: parent
        running: false

        ImageParticle {
            source: "qrc:///particleresources/star.png"
            alpha: 0
            colorVariation: 0.7
        }

        Emitter {
            id: pulseEmitter1
            x: grid.x + 25
            y: grid.y + 25
            emitRate: 200
            lifeSpan: 4000
            enabled: false
            velocity: AngleDirection{magnitude: 128; magnitudeVariation: 128; angle: 45; angleVariation: 45}
            size: 12
            sizeVariation: 12
        }

        Emitter {
            id: pulseEmitter2
            x: grid.x + grid.width + 30
            y: grid.y + grid.height + 30
            emitRate: 200
            lifeSpan: 5000
            lifeSpanVariation: 500
            enabled: false
            velocity: AngleDirection{magnitude: 128; magnitudeVariation: 128; angle: 225; angleVariation: 45}
            size: 12
            sizeVariation: 8
        }
        Emitter {
            id: pulseEmitter3
            x: grid.x + 30
            y: grid.y + grid.height + 30
            emitRate: 600
            lifeSpan: 5000
            lifeSpanVariation: 500
            enabled: false
            velocity: AngleDirection{magnitude: 128; magnitudeVariation: 128; angle: 315; angleVariation: 45}
            size: 12
            sizeVariation: 8
        }
        Emitter {
            id: pulseEmitter4
            x: grid.x + grid.width + 30
            y: grid.y + 30
            emitRate: 400
            lifeSpan: 500
            lifeSpanVariation: 500
            enabled: false
            velocity: AngleDirection{magnitude: 128; magnitudeVariation: 128; angle: 135; angleVariation: 45}
            size: 12
            sizeVariation: 8
        }

        Timer {
            id: timer
            interval: 7000
            onTriggered: {
                particles.reset()
                particles.stop()
            }
            running: false
            repeat: false
        }

        Connections {
            target: raytracer
            onRenderingChanged: {
                if (!raytracer.rendering && settings.fireworksEnabled) {
                    particles.start()
                    timer.stop()

                    pulseEmitter1.pulse(150);
                    pulseEmitter2.pulse(130);
                    pulseEmitter3.pulse(160);
                    pulseEmitter4.pulse(110);

                    timer.start()
                }
            }
        }
    }
    Label {
        id: rect
        x: window.width + rect.width
        y: window.height - rect.height
        text: "Rendering"

        SequentialAnimation {
            id: anim
            running: false
            NumberAnimation { target: rect; property: "x"; easing.type: Easing.OutExpo; to:  window.width - rect.width; duration: 400 }
            PauseAnimation { duration: 600 }
            NumberAnimation { target: rect; property: "x"; to: window.width+rect.width; duration: 200 }
        }

        Connections {
            target: raytracer
            onRenderingChanged: {
                if (raytracer.rendering) {
                    rect.x = window.width + rect.width
                    anim.start()

                }
            }
        }
    }
}
