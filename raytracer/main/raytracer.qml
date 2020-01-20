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
    color: "black"
    title: "Freeside Raytracer"
    minimumWidth: 1024
    minimumHeight: 768

    Connections {
        target: raytracer

        onSelectedObjectChanged: {
            informationWindow.visible = true
        }
    }

    Component.onCompleted: {
        window.showMaximized()
    }

    FontLoader {
        id: glyphs
        source: "fontello"
    }

    FontLoader {
        id: aircraft
        source: "B162"
    }

    Rectangle {
        id: leftRectangle
        Layout.rowSpan: 1
        color: "transparent"
        x: 0
        y: 0
        width: Math.max(300, 0.2 * window.width)
        height: window.height

        InformationWindow {
            id: informationWindow
            anchors.fill: parent
        }
    }

    Rectangle {
        id: middleRectangle
        color: "transparent"
        x: leftRectangle.visible ? leftRectangle.width : 0
        width: window.width - (leftRectangle.visible ? leftRectangle.width : 0) - (rightRectangle.visible ? rightRectangle.width : 0)
        height: window.height

        property int viewportWidth: Math.floor(settings.fullscreenEnabled ? middlePane.width : middlePane.width / 2) - 20
        property int viewportheight: Math.floor(settings.fullscreenEnabled ? middlePane.height : middlePane.height / 2) - 20

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
            id: middlePane
            Material.background: "gray"
            anchors.right: parent.right
            anchors.rightMargin: 87
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 102
            anchors.top: parent.top
            anchors.topMargin: 20

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
                anchors.top: parent.top
                anchors.left: parent.left

                width: middleRectangle.viewportWidth
                height: middleRectangle.viewportheight

                visible: !settings.fullscreenEnabled

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
                }
            }

            ImageItem {
                id: liveImageItem2
                anchors.right: parent.right
                anchors.top: parent.top

                width: middleRectangle.viewportWidth
                height: middleRectangle.viewportheight

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

                anchors.bottom: parent.bottom
                anchors.left: parent.left

                width: middleRectangle.viewportWidth
                height: middleRectangle.viewportheight

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

                anchors.right: parent.right
                anchors.bottom: parent.bottom

                width: middleRectangle.viewportWidth
                height: middleRectangle.viewportheight

                function updateWireframe() {
                    if (width < 0 || height < 0) return;

                    raytracer.setViewportSize(liveImageItem4.width, liveImageItem4.height)
                    raytracer.wireframe()
                }

                onWidthChanged: {
                    updateWireframe()
                }

                onHeightChanged: {
                    updateWireframe()
                }

                Connections {
                    target: raytracer
                    onImageReady: liveImageItem4.setImage(image)
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: raytracer.selectObject(mouseX, mouseY)

                    onWheel: {
                        raytracer.fromZ += wheel.angleDelta.y * 1/8 / 50;
                        raytracer.wireframe()
                    }

                    property int startX
                    property int startY

                    onPressed: {
                        startX = mouseX
                        startY = mouseY
                    }

                    onPositionChanged: {

                        if (raytracer.selectedObject.id <= 0) return

                        cursorShape = Qt.PointingHandCursor
                        let deltaX =  mouse.x - startX
                        let deltaY =  mouse.y - startY

                        var z = 0
                        var y = -deltaY * 0.01

                        if (mouse.modifiers & Qt.ShiftModifier) {
                            z = -deltaY * 0.01
                            y = 0
                        }

                        if (informationWindow.moveButton.checked) {
                            raytracer.translate(deltaX * 0.01, y, z)
                        } else if (informationWindow.scaleButton.checked) {
                            raytracer.scale(1 + deltaX * 0.01, 1 + y, 1 + z)
                        } else if (informationWindow.rotXButton.checked) {
                            raytracer.rotate_x(deltaY * 0.01)
                        } else if (informationWindow.rotYButton.checked) {
                            raytracer.rotate_y(deltaX * 0.01)
                        }else if (informationWindow.rotZButton.checked) {
                            raytracer.rotate_z(deltaX * 0.01)
                        }

                        raytracer.wireframe()

                        startX = mouse.x
                        startY = mouse.y
                    }

                    onReleased: {
                        cursorShape = Qt.ArrowCursor
                    }

                }

                ColorOverlay {
                    anchors.fill: parent
                    source: parent
                    color: "#80400000"
                    visible: raytracer.rendering
                }
            }
        }

        Slider {
            id: slider
            y: 366
            height: 40
            width: middlePane.width
            anchors.horizontalCenter: middlePane.horizontalCenter
            anchors.top: middlePane.bottom

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
            height: middlePane.height
            anchors.verticalCenter: middlePane.verticalCenter
            anchors.left: middlePane.right


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
            anchors.right: middlePane.right

            antialiasing: true
            enabled: !raytracer.rendering
            onClicked: {
                raytracer.setViewportSize(liveImageItem4.width, liveImageItem4.height)
                raytracer.render()
            }
        }

        Label {
            id: element3
            text: liveImageItem4.width + "x" + middlePane.height
            anchors.horizontalCenter: middlePane.horizontalCenter
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
            anchors.horizontalCenter: middlePane.horizontalCenter
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
                leftRectangle.visible = !leftRectangle.visible
            }
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
                rightRectangle.visible = !rightRectangle.visible
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
                x: middlePane.x + 25
                y: middlePane.y + 25
                emitRate: 200
                lifeSpan: 4000
                enabled: false
                velocity: AngleDirection{magnitude: 128; magnitudeVariation: 128; angle: 45; angleVariation: 45}
                size: 12
                sizeVariation: 12
            }

            Emitter {
                id: pulseEmitter2
                x: middlePane.x + middlePane.width + 30
                y: middlePane.y + middlePane.height + 30
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
                x: middlePane.x + 30
                y: middlePane.y + middlePane.height + 30
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
                x: middlePane.x + middlePane.width + 30
                y: middlePane.y + 30
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

    Rectangle {
        visible: true
        id: rightRectangle
        anchors.right: parent.right
        color: "transparent"
        width: 250
        height: window.height

        SettingsWindow {
            id: settingsWindow
            anchors.fill: parent
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:1022;width:1280}
}
##^##*/
