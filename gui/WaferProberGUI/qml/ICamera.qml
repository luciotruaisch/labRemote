import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import QtMultimedia 5.8

import "qrc:settings.js" as Settings

import CVCamera 1.0
// import io.qt.examples.cvcamera 1.0

Item {

    width: Settings.image_width
    height: Settings.image_height

//    Camera {
//        id: camera
//        //imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

//        captureMode: CaptureViewfinder
//        imageCapture {
//            onImageCaptured: {
//            }
//        }

//        videoRecorder {
//             resolution: "640x480"
//             frameRate: 30
//        }
//    }

    CVCamera {
        id: camera
        size: "1280x720"
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
