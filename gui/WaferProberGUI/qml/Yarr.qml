import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4



Item {
    Layout.fillWidth: true

    ColumnLayout {
        id: column
        anchors.fill: parent


        Pane {
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent

                TextArea {
                    anchors.fill: parent
                    text: "Will be added in future update"
                }
            }
        }


        Pane {
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent

                Button {
                    text: "XY PARK"
                    onClicked: {
                        backend.run_cmd("XY_PARK")
                    }
                }

                Button {
                    text: "XY UNPARK"
                    onClicked: {
                        backend.run_cmd("XY_UNPARK")
                    }
                }

            }
        }
    }

}
