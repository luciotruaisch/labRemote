import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import QtMultimedia 5.8

import "qrc:settings.js" as Settings

Item {

    width: Settings.image_width
    height: Settings.image_height

    Camera {
        id: camera
        //imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

        imageCapture {            
            onImageCaptured: {
            }
        }

        videoRecorder {
             resolution: "640x480"
             frameRate: 30
        }
    }


    VideoOutput {
        source: camera
         anchors.fill: parent
        Layout.fillWidth: true
        focus: visible
    }

    Button {
        text: "Snapshot"
        onClicked: {
            if(camera.imageCapture.ready) {
                camera.imageCapture.captureToLocation(Settings.image_saved_path + Settings.add()+".png")
            }
        }
    }

}
