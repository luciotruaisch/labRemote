import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

import "qml" // Housing customized Items
import "settings.js" as Settings

import io.qt.examples.backend 1.0

ApplicationWindow {
    id: window
    visible: true
    width: Settings.image_width + 500
    height: Settings.image_height+ 500

    title: qsTr("Wafter Probing console table. " + width + " x " + height)

    property var withCamera: false


    BackEnd {
        id: backend

        onDeviceConnected: {
            txt_pos_x.text = Number(backend.getPosX()).toLocaleString(Qt.locale("en_US"), 'f', 3)
            txt_pos_y.text = Number(backend.getPosY()).toLocaleString(Qt.locale("en_US"), 'f', 3)
            txt_pos_z.text = Number(backend.getPosZ()).toLocaleString(Qt.locale("en_US"), 'f', 3)
        }

        onPositionChanged: {
            if(axis == 0) {
                txt_pos_x.text = Number(backend.getPosX()).toLocaleString(Qt.locale("en_US"), 'f', 3)
            } else if(axis == 1) {
                txt_pos_y.text = Number(backend.getPosY()).toLocaleString(Qt.locale("en_US"), 'f', 3)
            } else if (axis == 2) {
                txt_pos_z.text = Number(backend.getPosZ()).toLocaleString(Qt.locale("en_US"), 'f', 3)
            } else if (axis == 3) {
                txt_pos_x.text = Number(backend.getPosX()).toLocaleString(Qt.locale("en_US"), 'f', 3)
                txt_pos_y.text = Number(backend.getPosY()).toLocaleString(Qt.locale("en_US"), 'f', 3)
            } else {
            }
        }

        Component.onCompleted: {
            xyDeviceName = Settings.xy_device.toString()
        }

        onInfoUpdated: {
            output.append(message)
        }
    }

    onClosing: {
        if(motion_content.isContact) {
            backend.zContact = false
        }
        backend.dismiss()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Settings.margin

        RowLayout {

            Frame {
                // Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    id: left_top_frame
                    anchors.fill: parent

                    ICamera {  }

                    GroupBox {
                        title: "status report"
                        Layout.fillWidth: true
                        GridLayout {
                            anchors.fill: parent
                            columns: 4

                            Label { text: "  "}
                            Label { text: "X [mm]"}
                            Label { text: "Y [mm]"}
                            Label { text: "Z [mm]"}
                            Label { text: "Absolute location: "}
                            Label {
                                id: txt_pos_x
                                text: "0."
                            }
                            Label {
                                id: txt_pos_y
                                text: "0."
                            }
                            Label {
                                id: txt_pos_z
                                text: "0."
                            }

                            Label { text: "Current chip:" }
                            Label {
                                text: "1"
                            }
                        }
                    }
                }
            }


            Frame {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent

                    TabBar {
                        id: side_bar
                        position: TabBar.Header
                        Layout.fillWidth: true

                        TabButton {
                            Layout.fillWidth: true
                            text: "Motion"
                        }
                        TabButton {
                            Layout.fillWidth: true
                            text: "Measurement"
                        }
                        TabButton {
                            Layout.fillWidth: true
                            text: "Yarr"
                        }
                    }
                    StackLayout {
                        id: side_bar_view
                        currentIndex: side_bar.currentIndex
                        Layout.fillWidth: true

                        Motion {
                            id: motion_content
                        }

                        Measurement {}

                        Yarr {}

                    }
                }
            }

        }

        Frame {
            implicitHeight: 100
            Layout.fillWidth: true

            ScrollView {
                id: sv_text
                anchors.fill: parent
                contentWidth: output.paintedWidth
                contentHeight: output.paintedHeight
                clip: true

                TextEdit {
                    id: output
                    Layout.fillWidth: true
                    focus: true
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    text: "Program started..."
                }

                ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            }
        }
    }
}
