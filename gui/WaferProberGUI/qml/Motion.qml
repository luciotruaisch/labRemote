import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item {
    id: content
    Layout.fillWidth: true

    property int idx_frame: 0

    ColumnLayout {
        id: column
        anchors.fill: parent

        property var connectResult: -999
        Button {
            // anchors.horizontalCenter: column.parent.parent.horizontalCenter
            Layout.alignment: Qt.AlignHCenter
            text: "connect"
            onClicked: {
                // connectResult = backend.to_connect
                console.info(column.width)
            }
        }

        // set separate


        Frame{
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                Button {
                    text: "Set Separate"
                    onClicked: {
                        idx_frame = 1
                    }
                }
                Switch {
                    id: btn_is_contact
                    text: btn_is_contact.checked? "In  Contact": "In Separate"
                    contentItem: Text {
                        text: btn_is_contact.text
                        font: btn_is_contact.font
                        color: btn_is_contact.checked ? "red" : "green"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: btn_is_contact.indicator.width + btn_is_contact.spacing
                    }
                }
            }
        }

        StackLayout {
            currentIndex: idx_frame
            // motion control, in 3D...

            //the frame for controling x-y.
            Frame {
                enabled: !btn_is_contact.checked
                Layout.fillWidth: true
                ColumnLayout{
                    anchors.fill:parent

                    // general x-y controller, with graphics.
                    Frame {
                        // title: "Motion Controller"
                        Layout.fillWidth: true
                        GridLayout{
                            anchors.fill: parent
                            rows: 3
                            flow: GridLayout.TopToBottom
                            ToolButton {

                                Layout.row: 1
                                Layout.column: 1
                                // text: "Left"
                                contentItem: Image {
                                    fillMode: Image.Pad
                                    source: "qrc:images/arrow.png"
                                    transform: Rotation { origin.x: 20; origin.y: 20;  angle: 180 }
                                }
                            }
                            ToolButton {
                                Layout.row: 0
                                Layout.column: 2
                                // text: "Up"
                                contentItem: Image {
                                    fillMode: Image.Pad
                                    //horizontalAlignment: Image.AlignHCenter
                                    //verticalAlignment: Image.AlignVCenter
                                    source: "qrc:images/arrow.png"
                                    transform: Rotation { origin.x: 20; origin.y: 20;  angle: -90 }
                                }
                            }

                            //                Button {
                            //                    text: z_dir.checked? "Z-Down": "Z-Up"
                            //                    autoRepeat: true
                            //                    onClicked: {
                            //                        output.cursorPosition = output.lineCount
                            //                        output.append(output.lineCount  + ": don't push me, " + output.cursorPosition + " "
                            //                                      + output.contentHeight + " " + output.length)

                            //                    }
                            //                }
                            ToolButton {
                                Layout.row: 2
                                Layout.column: 2
                                // text: "Down"
                                contentItem: Image {
                                    fillMode: Image.Pad
                                    source: "qrc:images/arrow.png"
                                    transform: Rotation { origin.x: 20; origin.y: 20;  angle: 90 }
                                }
                            }

                            ToolButton {
                                // text: "Right"
                                Layout.row: 1
                                Layout.column: 3
                                contentItem: Image {
                                    fillMode: Image.Pad
                                    horizontalAlignment: Image.AlignHCenter
                                    verticalAlignment: Image.AlignVCenter
                                    source: "qrc:images/arrow.png"
                                }
                            }

                            Button {
                                Layout.row: 0
                                Layout.column: 4
                                text: "Scan Wafer"
                            }
                            Button {
                                text: "Perform Measurements"
                            }
                        }
                    }

                    // set speed.
                    //        Frame {
                    //            Layout.fillWidth: true
                    //            GridLayout {
                    //                anchors.fill: parent
                    //                rows: 3
                    //                flow: GridLayout.TopToBottom
                    //                Button{
                    //                    Layout.row: 0
                    //                    Layout.column: 5
                    //                    text: "X speed"
                    //                }
                    //                Button{
                    //                    text: "Y speed"
                    //                }
                    //                Button{
                    //                    text: "Z speed"
                    //                }
                    //                TextField{
                    //                    text: "0"
                    //                    verticalAlignment: Text.AlignVCenter
                    //                    horizontalAlignment: Text.AlignHCenter
                    //                }
                    //                TextField{
                    //                    text: "0"
                    //                    verticalAlignment: Text.AlignVCenter
                    //                    horizontalAlignment: Text.AlignHCenter
                    //                }
                    //                TextField{
                    //                    text: "0"
                    //                    verticalAlignment: Text.AlignVCenter
                    //                    horizontalAlignment: Text.AlignHCenter
                    //                }
                    //                Repeater{
                    //                    model: 3
                    //                    Label {
                    //                        text: "mm/s"
                    //                    }
                    //                }
                    //            }
                    //        }

                    // move x-y station, grouped with pre-defined functions.
                    Frame {
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

                            Button {
                                text: "mv absolute"
                            }
                            TextField {
                                text: "0."
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                text: "0."
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            // third row
                            Button {
                                text: "mv relative"
                            }
                            TextField {
                                text: "0."
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            TextField {
                                text: "0."
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            // functions for X-Y axis
                            Button {
                                id: btn_xy_func
                                text: "X-Y Functions"
                                onClicked: menu_xy.open()
                                Menu {
                                    id: menu_xy
                                    MenuItem {
                                        text: "GO HOME"
                                        onTriggered: {
                                            //lbl_xy_func.text = text
                                        }
                                    }
                                    MenuItem {
                                        text: "GO MIDDLE"
                                    }
                                    MenuSeparator{
                                        visible: true
                                    }

                                    MenuItem {
                                        text: "SCAN X"
                                    }
                                    MenuItem {
                                        text: "SCAN Y"
                                    }
                                    MenuSeparator{
                                        visible: true
                                    }
                                    MenuItem {
                                        text: "TEST X"
                                    }
                                    MenuItem {
                                        text: "TEST Y"
                                    }
                                }
                            }

                            // functions for Z-axis
                            Button {
                                id: btn_z_func
                                text: "Z Functions"
                                onClicked: menu_z.open()
                                Menu {
                                    id: menu_z
                                    //                        MenuItem {
                                    //                            text: "MV CONTACT"
                                    //                        }
                                    //                        MenuItem {
                                    //                            text: "MV SEPARATE"
                                    //                        }
                                    //                        MenuSeparator{
                                    //                            visible: true
                                    //                        }
                                    MenuItem {
                                        text: "GO TOP"
                                    }
                                    MenuItem {
                                        text: "GO BOTTOM"
                                    }
                                    MenuSeparator{
                                        visible: true
                                    }
                                    MenuItem {
                                        text: "CALIBRATE"
                                    }
                                }
                            }


                        }
                    } //END move x-y

                }
            }

            // the frame that controls Z axis.
            Frame {
                Layout.fillWidth: true
                ColumnLayout {
                    anchors.fill: parent

                    GroupBox {
                        title: "set speed"
                        Layout.fillWidth: true
                        RowLayout {
                            anchors.fill: parent
                            Button{
                                text: "Z speed"
                            }
                            TextField{
                                text: "0"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            Label {
                                text: "mm/s"
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
                                contentItem: Text {
                                    text: btn_z_up.text
                                    font: btn_z_up.font
                                    color: "red"
                                }
                            }
                            Button {
                                id: btn_z_down
                                text: "Go Down"
                                contentItem: Text {
                                    text: btn_z_down.text
                                    font: btn_z_down.font
                                    color: "green"
                                }
                            }
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Button {
                            text: "Back"
                            // Layout.alignment: Qt.AlignHCenter
                            onClicked: {
                                idx_frame = 0
                            }
                        }
                        Button {
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
            text: "Are you sure needles are in contact of the chip?"
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            btn_is_contact.checked = true
        }
        onRejected: {

        }
    }

}
