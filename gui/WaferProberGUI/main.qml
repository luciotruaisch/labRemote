import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.0

import QtQuick.Extras 1.4

import "qml" // Housing customized Items

//import "settings.js" as Settings
import "qrc:settings.js" as Settings

import qt.wafer.backend 1.0
import qt.wafer.FileIO 1.0
import qt.wafer.objectdetection 1.0
import qt.wafer.CVCamera 1.0
import qt.wafer.CalibrateZ 1.0

ApplicationWindow {
    id: window
    visible: true
    width: Settings.image_width + 600
    height: Settings.image_height+ 600

    title: qsTr("Wafter Probing console table. " + width + " x " + height)

    property var withCamera: false
    property var with_correction: false
    property var calibrateAllChips: false
    property var yOffSet: -0.3

    // handy functions
    function go2chip(chip_id){
        var chip_axises = Settings.get_chip_axis(chip_id)
        var cmd_x = "MA X " + chip_axises.xAxis.toString()
        var cmd_y = "MA Y " + chip_axises.yAxis.toString()
        backend.run_cmd(cmd_x)
        backend.run_cmd(cmd_y)
        if(with_correction) {
            backend.run_cmd("ENDCHIP")
        }
    }
    function goNextChip(){
        go2chip(1+Settings.find_chip_number(current_chip_id.text))
    }
    function goPrevChip(){
        go2chip(Settings.find_chip_number(current_chip_id.text) - 1 )
    }

    function update_position(){
        txt_pos_x.text = Number(backend.getPosX()).toLocaleString(Qt.locale("en_US"), 'f', 3)
        txt_pos_y.text = Number(backend.getPosY()).toLocaleString(Qt.locale("en_US"), 'f', 3)
        txt_pos_z.text = Number(backend.getPosZ()).toLocaleString(Qt.locale("en_US"), 'f', 3)
    }

    CVCamera {
        id: camera
    }

    FileIO {
        id: height_input
        source: Settings.height_table.input_name
        onError: console.log(msg)
    }

    ObjectDetection {
        id: object_detection
        objectName: "qrc:images/RD53A_template_less.png"
        onCorrectionGenerated: {
            // change pixel to mm.
            dx *= 0.002
            dy *= -0.002
            // dy -= yOffSet
            var dx_str = dx.toLocaleString(Qt.locale("en_US"), 'f', 3)
            var dy_str = dy.toLocaleString(Qt.locale("en_US"), 'f', 3)
            if(with_correction) {
                backend.run_cmd("MR X "+ dx_str)
                backend.run_cmd("MR Y "+ dy_str)
            } else {
                motion_content.txt_rel_x.text = dx_str
                motion_content.txt_rel_y.text = dy_str
            }
            console.log("automated correction is:",dx_str, dy_str)
            console.log("Z corrections are: ", Settings.height_table.get(current_chip_id.text))
        }
    }


    BackEnd {
        id: backend

        onDeviceConnected: {
            update_position()
            // calibrate using previous results.
            Settings.update_true_chip_table(Settings.find_chip_number(Settings.chip_id_for_calibration),
                                            Settings.chip_x_for_calibration,
                                            Settings.chip_y_for_calibration
                                            )
            current_chip_id.text = Settings.find_chip_ID(Number(txt_pos_x.text), Number(txt_pos_y.text))
        }

        onPositionChanged: {
            update_position()
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

        onChipArrived: {
            object_detection.dstImage(camera.cvImage)
        }

        onSrcImageArrived: {
            object_detection.setSourceImage(camera.cvImage)
        }

        onChipArrivedForCalibrateZ: {
            autoZcal.start()
        }
    }

    // calibration for focus. measurement of height.
    CalibrateZ {
        id: autoZcal
        motionHandle: backend
        camera: camera
        onFocusFound: {
            update_position()
            Settings.height_table.update(current_chip_id.text, txt_pos_z.text)
            if(calibrateAllChips){
                goNextChip()
                backend.run_cmd("ENDFORCALIBRATEZ")
            }
        }
    }


    onClosing: {
        if(motion_content.connectResult == 0 && motion_content.isContact) {
            backend.zContact = false
        }
        if(motion_content.connectResult == 0) backend.dismiss()

        //console.log(Settings.real_chip_table.output())
        //        var result = real_chip_input.write(Settings.real_chip_table.output())
        //        if(result){
        //            console.log("Real Chip Table is written.")
        //        } else {
        //            console.log("Cannot write to File.")
        //        }
        autoZcal.dismiss()

        // console.log(Settings.height_table.output())
        var result = height_input.write(Settings.height_table.output())
        if(result){
            console.log("Height Table is written.")
        } else {
            console.log("Cannot write to File: ", Settings.height_table.input_name)
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
//                            onReadyForChipCorrection: {
//                                object_detection.dstImage(camera.cvImage)
//                            }
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
