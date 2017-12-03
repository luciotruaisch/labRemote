import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

import "qml" // Housing customized Items
//import "settings.js" as Settings
import "qrc:settings.js" as Settings

import qt.wafer.backend 1.0
import qt.wafer.FileIO 1.0
import qt.wafer.objectdetection 1.0

import CVCamera 1.0

ApplicationWindow {
    id: window
    visible: true
    width: Settings.image_width + 600
    height: Settings.image_height+ 600

    title: qsTr("Wafter Probing console table. " + width + " x " + height)

    property var withCamera: false

    CVCamera {
        id: camera
    }

    FileIO {
        id: real_chip_input
        source: Settings.real_chip_table.input_name
        onError: console.log(msg)
    }

    ObjectDetection {
        id: object_detection
        onCorrectionGenerated: {
            console.log("I will correct for", dx, dy)
            // change pixel to mm.
            dx *= 0.002
            dy *= 0.002
            motion_content.corret_xy(dx, dy)
        }
    }


    BackEnd {
        id: backend

        onDeviceConnected: {
            txt_pos_x.text = Number(backend.getPosX()).toLocaleString(Qt.locale("en_US"), 'f', 3)
            txt_pos_y.text = Number(backend.getPosY()).toLocaleString(Qt.locale("en_US"), 'f', 3)
            txt_pos_z.text = Number(backend.getPosZ()).toLocaleString(Qt.locale("en_US"), 'f', 3)
            // calibrate using previous results.
            Settings.update_true_chip_table(Settings.chip_id_for_calibration,
                                            Settings.chip_x_for_calibration,
                                            Settings.chip_y_for_calibration
                                            )
            current_chip_id.text = Settings.find_chip_ID(Number(txt_pos_x.text), Number(txt_pos_y.text))

            // load real chip table
            Settings.real_chip_table.read(real_chip_input.read())
            isConnected = true
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
            if(axis != 2){
                current_chip_id.text = Settings.find_chip_ID(Number(txt_pos_x.text), Number(txt_pos_y.text))
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
        if(motion_content.connectResult == 0 && motion_content.isContact) {
            backend.zContact = false
        }
        if(motion_content.connectResult == 0) backend.dismiss()
        console.log(Settings.real_chip_table.output())
        var result = real_chip_input.write(Settings.real_chip_table.output())
        if(result){
            console.log("Real Chip Table is written.")
        } else {
            console.log("Cannot write to File.")
        }
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

                    RowLayout{

                        Button {
                            id: btn_set_source
                            text: "set source"
                            onClicked: {
                                object_detection.setSourceImage(camera.cvImage)
                            }
                        }

                        Button {
                            id: btn_set_dst
                            text: "set destination"
                            onClicked: {
                                object_detection.dstImage(camera.cvImage)
                            }
                        }
                    }

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
                                id: current_chip_id
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
                            onReadyForChipCorrection: {
                                object_detection.setSourceImage(camera.cvImage)
                            }
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
