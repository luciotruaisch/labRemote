#ifndef CALIBRATEZ_H
#define CALIBRATEZ_H

#include <QObject>

#include "backend.h"
#include "CVCamera.h"


class CalibrateWorker : public QObject {
    // the worker that does the work.
    Q_OBJECT
public:
    explicit CalibrateWorker(MotionController* mc, CVCamera* camera);

public slots:
    void start(); // start timer
    void startRun() { m_running = true; } // start running
    void stop();

private slots:
    void run();

signals:
    void focusPointFound(double maxAbsZ);

protected:
    MotionController* m_ctrl;
    CVCamera* m_camera;
    QTimer* m_timer;
    bool m_running;
};

class CalibrateZ : public QObject
{
    Q_OBJECT

    Q_PROPERTY(BackEnd* motionHandle READ motionHandle WRITE setMotionHandle NOTIFY motionHandleChanged)
    Q_PROPERTY(CVCamera* camera READ camera WRITE setCamera NOTIFY cameraChanged)

public:
    explicit CalibrateZ(QObject* parent = nullptr);

    // loop over whole wafer
    Q_INVOKABLE void startLoop();

    // start calibration at current location.
    Q_INVOKABLE void start() { m_worker->startRun(); }
    Q_INVOKABLE void stop(){
        m_worker->stop();
    }
    Q_INVOKABLE void dismiss(){
        m_calibThread->quit();
    }

    static double calFeature(cv::Mat image);
    static double calFeature(QVariant input);
public:
    BackEnd* motionHandle();
    void setMotionHandle(BackEnd* backend);

    CVCamera* camera();
    void setCamera(CVCamera* camera);

public slots:
    void receiveFoundedFocus(double maxAbsZ);


signals:
    void motionHandleChanged();
    void cameraChanged();


private:
    CalibrateWorker* m_worker;
    QThread* m_calibThread;

    BackEnd* m_backend;
    CVCamera* m_camera;
};

#endif // CALIBRATEZ_H
