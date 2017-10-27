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


    BackEnd {
        id: backend
        onPosXChanged: {
            txt_pos_x.text = Number(backend.getPosX).toLocaleString()
        }
        onPosYChanged: {
            txt_pos_y.text = Number(backend.getPosY).toLocaleString()
        }
        onPosZChanged: {
            txt_pos_z.text = Number(backend.getPosZ).toLocaleString()
        }
    }

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

//                    ToolButton {
//                        anchors.left: parent.left
//                        contentItem: Image {
//                            source: "images/bio-photo.jpg"
//                        }
//                    }

                    ICamera {

                    }
//                    Image {
//                        width: Settings.image_width
//                        height: Settings.image_height
//                        // anchors.fill: parent
//                        id: photoPreview

//                        source: "qrc:images/bio-photo.jpg"
//                        sourceSize.width: Settings.image_width
//                        sourceSize.height: Settings.image_height

//                    }


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
//                            BusyIndicator {

//                            }
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

//                function ensureVisible(r) {
//                    if(contentX >= r.x) contentX = r.x;
//                    else if (contentX + width <= r.x + r.width) contentX = r.x + r.width - width;

//                    if(contentY >= r.y) contentY = r.y;
//                    else if (contentY + height <= r.y + r.height) contentY = r.y + r.height - height;
//                }


                TextEdit {
                    id: output
                    Layout.fillWidth: true
                    focus: true
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    text: "Program started..."
//                    onCursorRectangleChanged: sv_text.ensureVisible(cursorRectangle)
                }

                ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            }
        }
    }
}
