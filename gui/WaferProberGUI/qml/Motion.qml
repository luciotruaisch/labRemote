import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item {
    id: content
    Layout.fillWidth: true

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

        GroupBox {
            title: "move x-y"
            Layout.fillWidth: true
            GridLayout {
                id: grid
                anchors.fill: parent

                columns: 3
                Label { text: "   " }
                Label { text: "X [mm]" }
                Label { text: "Y [mm]" }

                Button {
                    text: "mv absolute"
                }
                TextField {
                    text: grid.width
                }
                TextField {
                    text: right_frame.width
                }
                // third row
                Button {
                    text: "mv relative"
                }
                TextField {
                    text: side_bar.width
                }
                TextField {
                    text: "0."
                }
                // fourth row
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

                Button {
                    id: btn_z_func
                    text: "Z Functions"
                    onClicked: menu_z.open()
                    Menu {
                        id: menu_z
                        MenuItem {
                            text: "MV CONTACT"
                        }
                        MenuItem {
                            text: "MV SEPARATE"
                        }
                        MenuSeparator{
                            visible: true
                        }
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

        GroupBox {
            title: "Wafer Probing Functions"
            Layout.fillWidth: true
            GridLayout {
                id: grid_func
                anchors.fill: parent
                Layout.fillWidth: true
                columns: 3
                Button {
                    text: "Scan Wafer"
                }
                Button {
                    text: "Perform Measurements"
                }
            }
        }
    }
}
