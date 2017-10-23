import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "qml" // Housing customized Items

import io.qt.examples.backend 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 750
    height: 750
    title: qsTr("Wafter Probing console table")

    BackEnd {
        id: backend
    }


//    header: ToolBar {
//        RowLayout {
//            anchors.fill: parent
//            anchors.margins: spacing
//            Label {
//                text: UI.label
//            }
//            Item { Layout.fillWidth: true }
//            CheckBox {
//                id: enabler
//                text: "Enabled"
//                checked: true
//            }
//        }
//    }

    property int margin: 5
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
                            Label { text: "absolute location: "}
                            Label { text: "0." }
                            Label { text: "0." }
                            Label {
                                text: "In Contact"
                                color: "green"
                            }
                            Label { text: "current chip:" }
                            Label { text: "1" }
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

                        Motion {}

                        Measurement {}

                        Yarr {}

                    }
                }
            }



        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            TextField {
                id: output
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 150
                text: "Output field"
            }
        }
    }


/**    Page1 {
    }
**/
}
