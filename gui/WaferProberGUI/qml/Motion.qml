import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import "qrc:settings.js" as Settings

Item {
    id: root
    Layout.fillWidth: true

    property int idx_frame: 0
    property var connectResult: -999
    property var isContact: false
    property var checkedSeparation: false
    property string command: " "

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
                    // Layout.alignment: Qt.AlignHCenter
                    // anchors.horizontalCenter: parent.horizontalCenter
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

                        } else if(connectResult == 1) {
                            output.append("XY station failed connection\n")
                            output.append("check if "+ Settings.xy_device +" is avaiable!")
                        } else {
                            output.append("Z station not connected. Check Ethernet. " + output.cursorPosition)
                            // output.cursorPosition = output.lineCount
                        }
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
                    GroupBox {
                        title: "Control NEEDLE"
                        Layout.fillWidth: true
                        GridLayout{
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

                                autoRepeat: true
                                onClicked: {
                                    if(isContact) go_separate()
                                    command = "MR X " + (txt_inc_x.text).toString()
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

                                autoRepeat: true
                                onClicked: {
                                    if(isContact) go_separate()                                   
                                    command = "MR Y " + (-1*txt_inc_y.text).toString()
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

                                autoRepeat: true
                                onClicked: {
                                    if(isContact) go_separate()
                                    command = "MR Y " + (txt_inc_y.text).toString()
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

                                autoRepeat: true
                                onClicked: {
                                    if(isContact) go_separate()
                                    command = "MR X " + (-1*txt_inc_x.text).toString()
                                    backend.run_cmd(command)
                                }
                            }
                            Button {
                                Layout.row: 0
                                Layout.column: 4
                                text: "Scan Wafer"
                            }
                            Button {
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
//                                        text: "TEST X"
//                                        onTriggered: {
//                                            if(isContact) go_separate()
//                                            backend.testXY = 0
//                                        }
//                                    }
//                                    MenuItem {
//                                        text: "TEST Y"
//                                        onTriggered: {
//                                            if(isContact) go_separate()
//                                            backend.testXY = 1
//                                        }
//                                    }
                                }
                            }
                        }
                    }

                    // move x-y station
                    Pane {
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
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                id: txt_abs_y
                                text: "0."
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
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                id: txt_rel_y
                                text: "0."
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
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                onEditingFinished: {
                                    backend.speedX = txt_speed_x.text.toString()
                                }
                            }
                            TextField {
                                id: txt_speed_y
                                text: Settings.speed_y
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
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                id: txt_inc_y
                                text: Settings.incre_y
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    } //END move x-y

                }
            }

            // the frame that controls Z axis.
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
                                text: "Z speed"
                            }
                            TextField{
                                id: txt_speed_z
                                text: "0.1"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                onEditingFinished: {
                                    backend.speedZ = txt_speed_z.text.toString()
                                }
                            }
                            Label {
                                text: "mm/s"
                            }
                            Button{
                                text: "Z separation"
                                onClicked: {
                                    backend.zSep = txt_sep_z.text.toString()
                                }
                            }
                            TextField{
                                id: txt_sep_z
                                text: "0.700"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            Label {
                                text: "mm"
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
                                //autoRepeat: true
                                contentItem: Text {
                                    text: btn_z_up.text
                                    font: btn_z_up.font
                                    color: "red"
                                }
                                onClicked: {
                                    command = "MR Z " + txt_speed_z.text.toString()
                                    backend.run_cmd(command)
                                }
                            }
                            Button {
                                id: btn_z_down
                                text: "Further"
                                Layout.fillWidth: true
                                //autoRepeat: true
                                contentItem: Text {
                                    text: btn_z_down.text
                                    font: btn_z_down.font
                                    color: "green"
                                }
                                onClicked: {
                                    command = "MR Z " + (-1 * txt_speed_z.text).toString()
                                    backend.run_cmd(command)
                                }
                            }

                            Button {
                                id: btx_z_calibrate
                                text: "CalibrateZ"
                                Layout.fillWidth: true
                                onClicked: {
                                    backend.calibrateZ()
                                }
                            }

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
                                }
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
            txt_speed_z.text = 0.7
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

}
