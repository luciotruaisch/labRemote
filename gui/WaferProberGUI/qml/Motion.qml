import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item {
    id: root
    Layout.fillWidth: true

    property int idx_frame: 0
    property var connectResult: -999
    property var isContact: false
    property var checkedSeparation: false

    signal xyPositionChanged()
    signal zPositionChanged()

    function go_separate() {
        if(isContact)   {

            backend.rel_z = -1*backend.zSep
            isContact = false
        }
        if(btn_is_contact.checked)  btn_is_contact.checked = false

    }

    function go_contact() {
        if(!isContact) {
            // backend.zContact = true
            backend.rel_z = backend.zSep
            isContact = true
        }
        if(!btn_is_contact.checked){
            btn_is_contact.checked = true
        }
    }



    ColumnLayout {
        id: column
        anchors.fill: parent


        Button {            
            Layout.alignment: Qt.AlignHCenter
            text: "connect"
            onClicked: {
                connectResult = backend.to_connect
                if (connectResult == 0) {
                    output.insert(0, "Connection is established\n")

                    // set current location and speed
                    root.xyPositionChanged()
                    root.zPositionChanged()

                    txt_abs_x.text = txt_pos_x.text
                    txt_abs_y.text = txt_pos_y.text

                    // set speed to default value
                    backend.speedX = txt_speed_x.text.toString()
                    backend.speedY = txt_speed_y.text.toString()
                    backend.speedZ = txt_speed_z.text.toString()
                } else if(connectResult == -1) {
                    output.insert(0, "XY station failed connection\n")
                } else {
                    output.insert(0, "Z station not connected. Check Ethernet\n")
                }

            }
        }

        Pane {
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                Button {
                    enabled: connectResult==0
                    text: "Set Separate"
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
                        console.log("Is checked: " + btn_is_contact.checked)
                        if(isContact && ! btn_is_contact.checked) {
                            go_separate()
                            console.log("Status " + isContact)
                        }
                        if(!isContact && btn_is_contact.checked) {
                            go_contact()
                            console.log("Status " + isContact)
                        }
                        console.log("Is checked: " + btn_is_contact.checked)
                    }
                }
            }
        }

        StackLayout {
            currentIndex: idx_frame
            // motion control, in 3D...

            //the frame for controling x-y.
            Pane {
                enabled: checkedSeparation
                Layout.fillWidth: true
                ColumnLayout{
                    anchors.fill:parent


                    // general x-y controller, with graphics.
                    Pane {
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
                                    backend.rel_x = txt_speed_x.text.toString()
                                    root.xyPositionChanged()
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
                                    backend.rel_y = (-1*txt_speed_y.text).toString()
                                    root.xyPositionChanged()
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
                                    backend.rel_y = txt_speed_y.text.toString()
                                    root.xyPositionChanged()
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
                                    backend.rel_x = (-1*txt_speed_x.text).toString()
                                    root.xyPositionChanged()
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
                                            backend.runSH
                                            root.xyPositionChanged()
                                        }
                                    }
                                    MenuItem {
                                        text: "GO MIDDLE"
                                        onTriggered: {
                                            if(isContact) go_separate()
                                            backend.runSM
                                            root.xyPositionChanged()
                                        }
                                    }
                                    MenuSeparator{
                                        visible: true
                                    }

                                    MenuItem {
                                        text: "SCAN X"
                                        onTriggered: {
                                            if(isContact) go_separate()

                                            backend.scanX = 1
                                            root.xyPositionChanged()
                                        }
                                    }
                                    MenuItem {
                                        text: "TEST X"
                                        onTriggered: {
                                            if(isContact) go_separate()
                                            backend.testXY = 0
                                            root.xyPositionChanged()
                                        }
                                    }
                                    MenuItem {
                                        text: "TEST Y"
                                        onTriggered: {
                                            if(isContact) go_separate()
                                            backend.testXY = 1
                                            root.xyPositionChanged()
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // move x-y station, grouped with pre-defined functions.
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
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            Label {
                                text: "Y [mm]"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            // move absolute
                            Button {
                                text: "mv absolute"
                                onClicked: {
                                    if(isContact) go_separate()
                                    backend.abs_x = txt_abs_x.text.toString()
                                    backend.abs_y = txt_abs_y.text.toString()
                                    fill_xy_pos()
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
                                    backend.rel_x = txt_rel_x.text.toString()
                                    backend.rel_y = txt_rel_y.text.toString()
                                    fill_xy_pos()
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
                            Button {
                                text: "SET SPEED"
                                onClicked: {
                                    backend.speedX = txt_speed_x.text.toString()
                                    backend.speedY = txt_speed_y.text.toString()
                                }
                            }

                            TextField {
                                id: txt_speed_x
                                text: "10."
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                id: txt_speed_y
                                text: "10."
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
                            Button{
                                text: "Z speed"
                                onClicked: {
                                    backend.speedZ = txt_speed_z.text.toString()
                                }
                            }
                            TextField{
                                id: txt_speed_z
                                text: "0.1"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
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
                                text: "Go Up"
                                Layout.fillWidth: true
                                //autoRepeat: true
                                contentItem: Text {
                                    text: btn_z_up.text
                                    font: btn_z_up.font
                                    color: "red"
                                }
                                onClicked: {
                                    backend.rel_z = txt_speed_z.text.toString()
                                    root.zPositionChanged()
                                }
                            }
                            Button {
                                id: btn_z_down
                                text: "Go Down"
                                Layout.fillWidth: true
                                //autoRepeat: true
                                contentItem: Text {
                                    text: btn_z_down.text
                                    font: btn_z_down.font
                                    color: "green"
                                }
                                onClicked: {
                                    backend.rel_z = (-1 * txt_speed_z.text).toString()
                                    root.zPositionChanged()
                                }
                            }
                            Button {
                                id: btn_z_stop
                                text: "Stop"
                                Layout.fillWidth: true
                                onClicked: {
                                    backend.stop
                                    root.zPositionChanged()
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
