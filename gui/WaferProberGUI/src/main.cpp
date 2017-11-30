#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "backend.h"
#include "FileIO.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<BackEnd>("qt.wafer.backend", 1, 0, "BackEnd");
    qmlRegisterType<FileIO, 1>("qt.wafer.FileIO", 1, 0, "FileIO");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
