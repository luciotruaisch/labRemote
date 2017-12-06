#ifndef CALIBRATEZ_H
#define CALIBRATEZ_H

#include <QObject>

#include "backend.h"
#include "objectdetection.h"

// Q_DECLARE_METATYPE(CVCamera)

class CalibrateWorker : public QObject {
    // the worker that does the work.
    Q_OBJECT
public:
    // explicit CalibrateWorker(MotionController* mc);
    explicit CalibrateWorker(MotionController* mc);

};

class CalibrateZ : public QObject
{
    Q_OBJECT

    // Q_PROPERTY(MotionController* motionHandle READ motionHandle WRITE setMotionHandle NOTIFY montionHandleChanged)
    // Q_PROPERTY(ObjectDetection* imageHandle READ imageHandle WRITE setImageHandle NOTIFY imageHandleChanged)
    Q_PROPERTY(QVariant camera READ camera WRITE setCamera NOTIFY cameraChanged)

public:
    explicit CalibrateZ(QObject* parent = nullptr);

    // set a refrence image, where microscope is focused.
    Q_INVOKABLE void setRefImage(QVariant input);

    // loop over whole wafer
    Q_INVOKABLE void startLoop();

    // start calibration at current location.
    Q_INVOKABLE void start();

public:
    QVariant cameraSource;

private:
    double m_refMean;

};

#endif // CALIBRATEZ_H
