import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import "qml" // Housing customized Items

import io.qt.examples.backend 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 850
    height: 750
    title: qsTr("Wafter Probing console table")

    BackEnd {
        id: backend
    }
//    property alias motion_content: motion_content

    onClosing: {
        if(motion_content.isContact) {
            backend.zContact = false
        }
        backend.dismiss
    }

    property int margin: 5
    // property alias txt_pos_x: txt_pos_x
    // property alias txt_pos_y: txt_pos_y

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: margin

        RowLayout {

            Frame {
                // Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    id: left_top_frame
                    anchors.fill: parent

                    ToolButton {
                        anchors.left: parent.left
                        contentItem: Image {
                            source: "images/bio-photo.jpg"
                        }
                    }

                    GroupBox {
                        title: "status report"
                        Layout.fillWidth: true
                        GridLayout {
                            anchors.fill: parent
                            columns: 3

                            Label { text: "  "}
                            Label { text: "X [mm]"}
                            Label { text: "Y [mm]"}
                            Label { text: "Absolute location: "}
                            Label {
                                id: txt_pos_x
                                text: "0."
                            }
                            Label {
                                id: txt_pos_y
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
                            onXyPostionChanged: {
                                txt_pos_x.text = Number(backend.getPosX).toLocaleString()
                                txt_pos_y.text = Number(backend.getPosY).toLocaleString()
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

                TextArea {
                    id: output
                    Layout.fillWidth: true
                    clip: true
                    text: "Program started..."
                    // verticalAlignment: TextEdit.AlignBottom
                }

                ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            }
        }
    }

/**    Page1 {
    }
**/
}
