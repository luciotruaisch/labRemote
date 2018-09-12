import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4

import "qrc:settings.js" as Settings

Item {
    id: root
    Layout.fillWidth: true

    property int idx_frame: 0
    property var connectResult: -999
    property var isContact: false
    property var checkedSeparation: false

    property var isCalibrated: false

    property var txt_rel_x: txt_rel_x
    property var txt_rel_y: txt_rel_y

    signal readyForChipCorrection()

    function go_separate() {
        if(isContact)   {
            backend.run_cmd("MR Z " + (-1*backend.zSep) )
            isContact = false
        }
        if(btn_is_contact.checked)  btn_is_contact.checked = false

    }

    function go_contact() {
        if(!isContact) {
            backend.run_cmd("MR Z " + backend.zSep )
            isContact = true
        }
        if(!btn_is_contact.checked){
            btn_is_contact.checked = true
        }
    }

    ColumnLayout {
        id: column
        anchors.fill: parent

        Pane {
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                Button {
                    text: "connect"
                    onClicked: {                        
                        connectResult = backend.connectDevice()
                        if (connectResult == 0) {
                            output.append("Connection is established\n")

                            txt_abs_x.text = txt_pos_x.text
                            txt_abs_y.text = txt_pos_y.text

                            // set speed to default value
                            backend.speedX = txt_speed_x.text.toString()
                            backend.speedY = txt_speed_y.text.toString()
                            backend.speedZ = txt_speed_z.text.toString()

                            //output.append("Now moving to minimum z position.") //TO DO
                            //var command = "MR Z " + txt_speed_z.text.toString() //don't know what to put for z increment yet
                            //backend.run_cmd(command)
                            //backend.run_cmd("CheckZmin")
                            //backend.run_cmd("END_CheckZmin")


                        } else if(connectResult == 1) {
                            output.append("XY station failed connection\n")
                            output.append("check if "+ Settings.xy_device +" is avaiable!")
                        } else {
                            output.append("Z station not connected. Check Ethernet. " + output.cursorPosition)
                        }
                    }
                }

                Button {
                    text: "Update Position"
                    onClicked: {
                        update_position()
                    }
                }

                Button {
                    text: "Stop"
                    onClicked: {
                        backend.stop()
                    }
                }
                Button {
                    text: "HELP"
                    onClicked:  {
                        Qt.openUrlExternally("https://github.com/xju2/labRemote/wiki")
                    }
                }
            }
        }


        Pane {
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                Button {
                    enabled: connectResult==0
                    text: "Set Contact Position"
                    onClicked: {
                        idx_frame = 1
                    }
                }
//                Button {
//                    text: "Set Files"
//                    onClicked: menu_files.open()
//                    Menu {
//                        id: menu_files
//                        MenuItem {
//                            text: "Height Calibration Files"
//                            onTriggered: {
//                                height_file_dialog.open()
//                            }
//                        }
//                    }
//                }

                Switch {
                    id: btn_is_contact
                    enabled: false
                    text: btn_is_contact.checked? "In  Contact": "In Separate"
                    contentItem: Text {
                        text: btn_is_contact.text
                        font: btn_is_contact.font
                        color: btn_is_contact.checked ? "red" : "green"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: btn_is_contact.indicator.width + btn_is_contact.spacing
                    }                    
                    onClicked: {
                        if(isContact && ! btn_is_contact.checked) {
                            go_separate()
                        }
                        if(!isContact && btn_is_contact.checked) {
                            go_contact()
                        }
                    }
                }
            }
        }

        StackLayout {
            currentIndex: idx_frame
            // motion control, in 3D...

            //the frame for controling x-y.
            Pane {
                // enabled: checkedSeparation
                Layout.fillWidth: true
                ColumnLayout{
                    anchors.fill:parent

                    // general x-y controller, with graphics.
                    Pane {
                        //title: "Control NEEDLE"
                        Layout.fillWidth: true
                        GridLayout{
                            id: ctr_grid
                            anchors.fill: parent
                            rows: 3
                            flow: GridLayout.TopToBottom
                            ToolButton {
                                // text: Left
                                enabled: !isContact
                                Layout.row: 1
                                Layout.column: 1
                                contentItem: Image {
                                    fillMode: Image.Pad
                                    source: "qrc:images/arrow.png"
                                    transform: Rotation { origin.x: 20; origin.y: 20;  angle: 180 }
                                }
                                autoRepeat: false
                                onClicked: {
                                    if(isContact) go_separate()
                                    var command = "MR X -" + (txt_inc_x.text).toString()
                                    backend.run_cmd(command)
                                }
                            }
                            ToolButton {
                                 // text: "Up"
                                enabled: !isContact
                                Layout.row: 0
                                Layout.column: 2
                                contentItem: Image {
                                    fillMode: Image.Pad
                                    source: "qrc:images/arrow.png"
                                    transform: Rotation { origin.x: 20; origin.y: 20;  angle: -90 }
                                }
                                autoRepeat: false

                                onClicked: {
                                    if(isContact) go_separate()                                   
                                    var command = "MR Y " + (txt_inc_y.text).toString()
                                    backend.run_cmd(command)
                                }
                            }

                            ToolButton {
                                // text: "Down"
                                enabled: !isContact
                                Layout.row: 2
                                Layout.column: 2
                                contentItem: Image {
                                    fillMode: Image.Pad
                                    source: "qrc:images/arrow.png"
                                    transform: Rotation { origin.x: 20; origin.y: 20;  angle: 90 }
                                }
                                autoRepeat: false

                                onClicked: {
                                    if(isContact) go_separate()
                                    var command = "MR Y -" + (txt_inc_y.text).toString()
                                    backend.run_cmd(command)
                                }
                            }

                            ToolButton {
                                // text: "Right"
                                enabled: !isContact
                                Layout.row: 1
                                Layout.column: 3
                                contentItem: Image {
                                    fillMode: Image.Pad
                                    horizontalAlignment: Image.AlignHCenter
                                    verticalAlignment: Image.AlignVCenter
                                    source: "qrc:images/arrow.png"
                                }
                                autoRepeat: false

                                onClicked: {
                                    if(isContact) go_separate()
                                    var command = "MR X " + (txt_inc_x.text).toString()
                                    backend.run_cmd(command)
                                }
                            }

                            // XY pre-defined functions.
                            Button {
                                Layout.row: 0
                                Layout.column: 5
                                id: btn_xy_func
                                text: "X-Y Functions"
                                onClicked: menu_xy.open()
                                Menu {
                                    id: menu_xy
                                    MenuItem {
                                        text: "GO HOME"
                                        onTriggered: {
                                            if(isContact) go_separate()
                                            backend.run_cmd("SH")
                                        }
                                    }
                                    MenuItem {
                                        text: "GO MIDDLE"
                                        onTriggered: {
                                            if(isContact) go_separate()
                                            backend.run_cmd("SM")
                                        }
                                    }
                                    MenuItem{
                                        text: "Load Wafer"
                                        onTriggered: {
                                            if(isContact) go_separate()
                                            backend.run_cmd("MA X 152.5")
                                            backend.run_cmd("MA Y 305")
                                        }
                                    }
                                    MenuItem {
                                        text: "SCAN X"
                                        onTriggered: {
                                            if(isContact) go_separate()
                                            txt_speed_x.text = 0.5
                                            backend.speedX = txt_speed_x.text.toString()
                                            backend.run_cmd("SCAN X")
                                        }
                                    }
//                                    MenuItem {
//                                        text: "Calibrate"
//                                        onTriggered: {
//                                            var chip_id = Number(current_chip_id.text)
//                                            Settings.real_chip_table.update(chip_id,
//                                                                            Number(txt_pos_x.text),
//                                                                            Number(txt_pos_y.text)
//                                                                            )
//                                            console.log(chip_id, "is corrected to:", txt_pos_x.text, txt_pos_y.text)
//                                        }
//                                        ToolTip.text: qsTr("Save current position as the true position for current chip.")
//                                        ToolTip.visible: hovered
//                                        ToolTip.delay: 1000
//                                        ToolTip.timeout: 4000
//                                    }
                                }
                            }

                            Button {
                                enabled: isCalibrated
                                text: "Go 2 Chip: "
                                onClicked: {
                                    if(isContact) go_separate()
                                    go2chip(Settings.find_chip_number(txt_chip_id.text))
                                }
                            }

                            Button {
                                enabled: isCalibrated
                                text: "Prev chip"
                                onClicked: {
                                    if(isContact) go_separate()
                                    goPrevChip()
                                }

                                ToolTip.text: qsTr("go to previous chip in numbering")
                                ToolTip.visible: hovered
                                ToolTip.delay: 1000
                                ToolTip.timeout: 4000
                            }

                            Button{
                                id: tog_with_cal
                                checkable: true
                                checked: false
                                text: "auto correction"
                                onClicked: {
                                    with_correction = tog_with_cal.checked
                                    console.log("with calibration: ",with_correction)
                                }
                                ToolTip.text: qsTr("In red (checked), apply auto-mated calibration, otherwise not")
                                ToolTip.visible: hovered
                                ToolTip.delay: 1000
                                ToolTip.timeout: 4000
                            }

                            TextField {
                                id: txt_chip_id
                                placeholderText: "chip ID"
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Button {
                                enabled: isCalibrated
                                text: "Next chip"
                                onClicked: {
                                    if(isContact) go_separate()
                                    goNextChip()
                                }

                                ToolTip.text: qsTr("go to next chip in numbering")
                                ToolTip.visible: hovered
                                ToolTip.delay: 1000
                                ToolTip.timeout: 4000
                            }
                        }
                    }

                    // Calibrate the x-y station
                    Pane {
                        enabled: connectResult==0
                        Layout.fillWidth: true
                        // calibrate x-y position.
                        GridLayout {
                            anchors.fill: parent
                            rows: 4
                            Button{
                                text: "calibrate XY"
                                onClicked: {
                                    if(isContact) go_separate()
                                    if(with_correction) {
                                        backend.run_cmd("MR Y " + yOffSet.toString())
                                        backend.run_cmd("ENDCALIBRATE")
                                        backend.run_cmd("MR Y " + (-1*yOffSet).toString())
                                    }
                                    var chip_num = Settings.find_chip_number(txt_chip_id_calibrate.text)
                                    Settings.update_true_chip_table(chip_num,
                                                                    Number(txt_chip_x_calibrate.text),
                                                                    Number(txt_chip_y_calibrate.text) + Number(yOffSet)
                                                                    )
                                    isCalibrated = true
                                    Settings.height_table.refID = txt_chip_id_calibrate.text
                                }
                                ToolTip.text: qsTr("Set a starting point! Make sure RD53 is in the image.")
                                ToolTip.visible: hovered
                                ToolTip.delay: 1000
                                ToolTip.timeout: 5000
                            }

                            TextField {
                                id: txt_chip_id_calibrate
                                placeholderText: "chip id"
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                text: Settings.chip_id_for_calibration
                                onTextChanged: {
                                    Settings.chip_id_for_calibration = txt_chip_id_calibrate.text
                                }
                            }

                            TextField {
                                id: txt_chip_x_calibrate
                                placeholderText: "x axis"
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                text: Settings.chip_x_for_calibration
                                onTextChanged: {
                                    Settings.chip_x_for_calibration = text
                                }
                            }
                            TextField {
                                id: txt_chip_y_calibrate
                                placeholderText: "y axis"
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                                text: Settings.chip_y_for_calibration
                                onTextChanged: {
                                    Settings.chip_y_for_calibration = text
                                }
                            }
                            Button {
                                Layout.row: 1
                                Layout.column: 0
                                id: btn_start_Z_calib
                                enabled: !isContact
                                text: "Calibrate Z"
                                onClicked:  {
                                    autoZcal.start()
                                }
                            }
                            Button {
                                enabled: !isContact
                                id: btn_stop_Z_calib
                                text: "STOP Z calib"
                                onClicked: {
                                    autoZcal.stop()
                                }
                            }
                            Button{
                                id: calib_all_chips
                                checkable: true
                                checked: false
                                text: "Calib All Z"
                                onClicked: {
                                    calibrateAllChips = calib_all_chips.checked
                                }
                                ToolTip.text: qsTr("In red (checked), calibrate-z will run over all chips, otherwise not")
                                ToolTip.visible: hovered
                                ToolTip.delay: 1000
                                ToolTip.timeout: 4000
                            }
//                            Button{
//                                id: calib_z_range
//                                checkable: true
//                                checked: false
//                                text: "Calib Z Range"
//                                onClicked: {
//                                   // backend.run_cmd("MA Z")
//                                }
//                                ToolTip.text: qsTr("Moves to the minimum Z, and checks the range with ./bin/findz_max_min txt file output")
//                                ToolTip.visible: hovered
//                                ToolTip.delay: 1000
//                                ToolTip.timeout: 4000
//                            }
                        }

                    }


                    // move x-y station
                    Pane {
                        enabled: connectResult==0
                        // title: "move x-y"
                        Layout.fillWidth: true
                        GridLayout {
                            id: grid
                            anchors.fill: parent

                            columns: 3
                            Label { text: "   " }
                            Label {
                                text: "X [mm]"
                                verticalAlignment: Label.AlignVCenter
                                horizontalAlignment: Label.AlignHCenter
                            }
                            Label {
                                text: "Y [mm]"
                                verticalAlignment: Label.AlignVCenter
                                horizontalAlignment: Label.AlignHCenter
                            }
                            // move absolute
                            Button {
                                text: "mv absolute"
                                onClicked: {
                                    if(isContact) go_separate()
                                    backend.run_cmd("MA X " + txt_abs_x.text.toString())
                                    backend.run_cmd("MA Y " + txt_abs_y.text.toString())
                                }
                            }
                            TextField {
                                id: txt_abs_x
                                text: "0."
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                id: txt_abs_y
                                text: "0."
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            // move relative
                            Button {
                                text: "mv relative"
                                onClicked: {
                                    if (isContact) go_separate()
                                    backend.run_cmd("MR X " + txt_rel_x.text.toString())
                                    backend.run_cmd("MR Y " + txt_rel_y.text.toString())
                                }
                            }
                            TextField {
                                id: txt_rel_x
                                text: "0."
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                id: txt_rel_y
                                text: "0."
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            // set speed
                            Label {
                                text: "SET SPEED"
                            }

                            TextField {
                                id: txt_speed_x
                                text: Settings.speed_x
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                onEditingFinished: {
                                    backend.speedX = txt_speed_x.text.toString()
                                }
                            }
                            TextField {
                                id: txt_speed_y
                                text: Settings.speed_y
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                onEditingFinished: {
                                    backend.speedY = txt_speed_y.text.toString()
                                }
                            }
                            // set increment
                            Label {
                                text: "SET INCREMENT"
                            }

                            TextField {
                                id: txt_inc_x
                                text: Settings.incre_x
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                id: txt_inc_y
                                text: Settings.incre_y
                                selectByMouse: true
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    } //END move x-y

                }
            }
            // id: calib_z_range
            //                                checkable: true
            //                                checked: false
            //                                text: "Calib Z Range"
            //                                onClicked: {
            //                                   // backend.run_cmd("FZrange")
            //                                }
            //                                ToolTyes_z_calibrated.open()ip.text: qsTr("Moves to the minimum Z, and checks the range with ./bin/findz_max_min txt file output")
            //                                ToolTip.visible: hovered
            //                                ToolTip.delay: 1000
            //                                ToolTip.timeout: 4000
            //                            }
            Pane {
                    Layout.fillWidth: true
                    ColumnLayout {
                        anchors.fill: parent

                        GroupBox {
                            title: "Settings"
                            Layout.fillWidth: true
                            GridLayout {
                                anchors.fill: parent
                                columns: 3
                                Label {
                                    text: "Z relative position increment" //Was labeled incorrectly. TO DO: Change id to txt_relpos_z
                                }
                                TextField{
                                    id: txt_speed_z
                                    text: "0.1"
                                    selectByMouse: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    onEditingFinished: {
                                        backend.speedZ = txt_speed_z.text.toString()
                                    }
                                }
                                Label {
                                    text: "mm"
                                }

                                Label {
                                    text: "Z speed (TO DO)" //TO DO: Need to figure out how to change speed
                                }
                                TextField{
                                    id: txt_zspeed
                                    text: "0.1"
                                    selectByMouse: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    onEditingFinished: {
                                        //backend.speedZ =
                                    }
                                }
                                Label {
                                    text: "mm/s"
                                }

                                Label {
                                    text: "Z separation"
                                }
                                TextField{
                                    id: txt_sep_z
                                    text: "0.700"
                                    selectByMouse: true
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    onEditingFinished: {
                                        backend.zSep = txt_sep_z.text.toString()
                                    }
                                }
                                Label {
                                    text: "mm"
                                }
                            }
                       }
                        GroupBox{
                            title: "CLOSER Z Fine Adjustment"
                            Layout.fillWidth: true
                            RowLayout{
                                anchors.fill: parent
                                Button{
                                    id: ten_micron
                                    text: "10um"
                                    Layout.fillWidth: true
                                    onClicked: {
                                        txt_speed_z.text = 0.01;
                                    }
                                }
                                Button{
                                    id: twenty_micron
                                    text: "20um"
                                    Layout.fillWidth: true
                                    onClicked: {
                                        txt_speed_z.text = 0.02;
                                        //var command = "MR Z 0.02"
                                        //backend.run_cmd(command)
                                    }
                                }
                                Button{
                                    id: thirty_micron
                                    text: "30um"
                                    Layout.fillWidth: true
                                    onClicked:{
                                        txt_speed_z.text = 0.03;
                                    }
                                }
                                Button{
                                    id: fifty_micron
                                    text: "50um"
                                    Layout.fillWidth: true
                                    onClicked:{
                                        txt_speed_z.text = 0.05;
                                    }
                                }
                                Button{
                                    id: hundred_micron
                                    text: "100um"
                                    Layout.fillWidth: true
                                    onClicked:{
                                        txt_speed_z.text = 0.1;
                                    }
                                }

                            }
                        }

                        GroupBox{
                            title: "controller"
                            Layout.fillWidth: true
                            RowLayout {
                                anchors.fill: parent
                                Button {
                                    id: btn_z_up
                                    text: "Closer"
                                    Layout.fillWidth: true
                                    contentItem: Text {
                                        text: btn_z_up.text
                                        font: btn_z_up.font
                                        color: "red"
                                    }
                                    onClicked: {
                                        if(parseFloat(txt_speed_z.text.toString()) >= 0.1){
                                             warning_closerZ.open()
                                        }
                                        else{
                                            var command = "MR Z " + txt_speed_z.text.toString()
                                            backend.run_cmd(command)
                                        }
                                    }
                                }

                                Button {
                                    id: btn_z_down
                                    text: "Further"
                                    Layout.fillWidth: true
                                    contentItem: Text {
                                        text: btn_z_down.text
                                        font: btn_z_down.font
                                        color: "green"
                                    }
                                    onClicked: {
                                        var command = "MR Z " + (-1 * txt_speed_z.text).toString()
                                        backend.run_cmd(command)
                                    }
                                }

                                  Button{
                                        id: find_z_min
                                        text: "Find Z min"
                                        Layout.fillWidth: true
                                        onClicked: {
                                            backend.run_cmd("FZMIN")
                                        }
                                    }  
                                  
//                                Button {
//                                    id: check_Z_cal
//                                    text: "Check Z calibration"
//                                    Layout.fillWidth: true
//                                    onClicked: {
//                                        backend.run_cmd("CheckZmin")
//                                        backend.run_cmd("END_CheckZmin")

//                                        if(backend.IsZCalibrated){
//                                            z_is_calibrated.open()
//                                        } else{
//                                            z_not_calibrated.open()
//                                        }
//                                    }
//                                }

//                                Button {
//                                    id: find_min_max_Z
//                                    text: "Recalibrate Z"
//                                    Layout.fillWidth: true
//                                    onClicked: {
//                                        warning_calibrateZ.open()
//                                    }
//                                }

                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            Button {
                                Layout.fillWidth: true
                                text: "Back"
                                // Layout.alignment: Qt.AlignHCenter
                                onClicked: {
                                    if(checkedSeparation) idx_frame = 0
                                    else {
                                        warning_dialog.open()
                                    }closer
                                }
                            }
                            Button {
                                Layout.fillWidth: true
                                text: "Confirm"
                                // Layout.alignment: Qt.AlignHCenter
                                onClicked: {
                                    contact_confirm.open()
                                }
                            }
                        }
                    }
                }
        }

            // the frame that controls Z axis.
    }

    Dialog {
        id: contact_confirm
        modal: true
        focus: true
        title: "Confirmation"
        Label {
            text: "Are you sure needles are in contact with the chip?"
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            isContact = true
            btn_is_contact.enabled = true
            btn_is_contact.checked = true
            checkedSeparation = true
            backend.IsAtContact = true
            txt_speed_z.text = txt_sep_z.text
            backend.speedZ = txt_sep_z.text
        }
        onRejected: {

        }
    }

    Dialog{
        id: warning_dialog
        modal: true
        focus: true
        title: "Confirmation"
        Label {
            text: "Leave without finishing calibrating contact position?"
        }
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            idx_frame = 0
        }
        onRejected: {
        }
    }

    Dialog {
        id: warning_calibrateZ
        modal: true
        focus: true
        title: "Confirmation"
        ColumnLayout{
            Label {
                text: "Sure to calibrate Z?"
            }
            Label{
                text: "It will move Z-axis to minimum and maximum location!"
            }

            Label {
                text: "Make sure the plane is lifted."
            }
            Label {
                text: "Click OK to proceed, otherwise Cancel."
            }
        }
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            // backend.calibrateZ()
            backend.run_cmd("FZrange")
        }
        onRejected: {

        }

    }

    Dialog {
        id: warning_closerZ
        modal: true
        focus: true
        title: "Confirmation"
        Label {
            text: "Are you sure you want to move the needles " + txt_speed_z.text.toString() + "mm closer to the chip?"
            //color: red
        }
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted:{
            var command = "MR Z " + txt_speed_z.text.toString()
            backend.run_cmd(command)
        }
        onRejected:{

        }
    }

    Dialog {
        id: z_is_calibrated
        modal: true
        focus: true
        title: "Z IS CALIBRATED"
        Label {
            text: "You may now adjust the Z stage."
        }

        standardButtons: Dialog.Ok

        onAccepted: {

        }
        onRejected: {

        }
    }

    Dialog {
        id: z_not_calibrated
        modal: true
        focus: true
        title: "Z IS NOT CALIBRATED"
        Label {
            text: "Controls for Z controller are locked. Please Recalibrate Z."
        }

        standardButtons: Dialog.Ok

        onAccepted: {

        }
        onRejected: {

        }
    }
}
