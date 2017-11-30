import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import QtMultimedia 5.8

import "qrc:settings.js" as Settings

import CVCamera 1.0

Item {

    property date currentDate: new Date()
    width: Settings.image_width
    height: Settings.image_height
    // height: Settings.image_height + btn_snap.height*2

    CVCamera {
        id: camera
    }

    ColumnLayout {

        Button {
            id: btn_snap
            text: "Snapshot"
            onClicked: {
                //            if(camera.imageCapture.ready) {
                //                camera.imageCapture.captureToLocation(Settings.image_saved_path + Settings.add()+".png")
                //            }
                camera.saveImage(Settings.image_saved_path +currentDate.toLocaleString()+"_"+Settings.add()+".png")
            }
        }

        VideoOutput {
            source: camera
            anchors.fill: parent
            Layout.fillWidth: true
            focus: visible
        }



    }

}
