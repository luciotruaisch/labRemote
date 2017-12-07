#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "backend.h"
#include "FileIO.h"
#include "ObjectDetection.h"
#include "CVCamera.h"
#include "CalibrateZ.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<BackEnd>("qt.wafer.backend", 1, 0, "BackEnd");
    qmlRegisterType<FileIO, 1>("qt.wafer.FileIO", 1, 0, "FileIO");
    qmlRegisterType<ObjectDetection, 1>("qt.wafer.objectdetection", 1, 0, "ObjectDetection");
    qmlRegisterType<CVCamera, 1>("qt.wafer.CVCamera", 1, 0, "CVCamera");
    qmlRegisterType<CalibrateZ, 1>("qt.wafer.CalibrateZ", 1, 0, "CalibrateZ");

    // qmlRegisterType<ChipVideoFilter, 1>("qt.wafer.chipvideofilter", 1, 0, "ChipFilter");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
