import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import QtMultimedia 5.8

import "qrc:settings.js" as Settings



Item {

    property date currentDate: new Date()
    width: Settings.image_width
    height: Settings.image_height + 100

    Button {
        id: btn_snap
        text: "Snapshot"
        onClicked: {
            camera.saveImage(Settings.image_saved_path + Settings.add()+"_"+currentDate.toLocaleString()+".png")
		}
	} 

    VideoOutput {
        source: camera
        anchors.fill: parent
        Layout.fillWidth: true
        focus: visible

    }

}
