#include "CalibrateZ.h"
#include "opencv2/core.hpp"

#include <QDebug>


Q_DECLARE_METATYPE(cv::Mat)

CalibrateWorker::CalibrateWorker(MotionController *mc, CVCamera *camera):
    m_ctrl(mc),
    m_camera(camera)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(run()));
    m_running = false;
    m_refMean = -1;
}

void CalibrateWorker::start(){
    m_timer->start(1500); // in 1 second
}

void CalibrateWorker::stop(){
    m_running = false;
}

void CalibrateWorker::run()
{
    if(m_running) {
        qInfo() << "Calibrate worker is working";
    }
    if(m_running && m_refMean > 0){
        // start to move chunk up and down to compare to mean value.
        m_ctrl->set_speed(2, 0.020);

        const double MAX_DISTANCE = 0.200;
        const int Max_TRIES = 10;

        bool foundFocus = true;
        double step = 0.02;
        double direction = 1; // 1 for increase; -1 for decrease

        double offset = 0.;

        double currentMean = CalibrateZ::calMean(m_camera->getCvImage());
        double previousMean = currentMean;
        double accumulateMovement = 0;
        int ntry = 1;
        int nFlip = 0;
        m_ctrl->get_pos_z();
        double absZ = m_ctrl->m_position[2];
        double minAbsZ = absZ;
        double minMean = currentMean;
        qInfo()<<"Starting point: " << minAbsZ << " " << minMean;
        while(m_running && ntry < Max_TRIES) {
            double moveZ = (step+offset)*direction;
            accumulateMovement += moveZ;
            if(accumulateMovement > MAX_DISTANCE) {
                qInfo()<<"moved more than 300 um!";
                foundFocus = false;
                break;
            }
            absZ += moveZ;
            m_ctrl->mv_abs(2, absZ);
            currentMean = CalibrateZ::calMean(m_camera->getCvImage());
            if(currentMean < 100) break;
            m_ctrl->get_pos_z();
            qInfo() << "Z: " << absZ << "("<< m_ctrl->m_position[2] << "); mean: " << currentMean;
            if(currentMean < minMean) {
                minMean = currentMean;
                minAbsZ = absZ;
            }
            if(currentMean > previousMean)
            {
                // qInfo()<<"change direction with smaller step";
                direction *= -1; // change direction
                offset = step;
                nFlip += 1;
                step *= 0.5;
                if(nFlip > 1) break;
            } else {
                offset = 0;
            }
            previousMean = currentMean;
            ntry ++;
        }
        m_ctrl->mv_abs(2, minAbsZ);
        qInfo()<<"Ending point: " << minAbsZ << " " << minMean;

        m_ctrl->get_pos_z();
        qInfo()<<"New Position: " << m_ctrl->m_position[2];
        if(foundFocus) emit focusPointFound();
        m_running = false;
    }
}


CalibrateZ::CalibrateZ(QObject* parent):
    QObject(parent)
{
    m_worker = nullptr;
    m_calibThread = nullptr;
    m_backend = nullptr;
    m_camera = nullptr;
}

BackEnd* CalibrateZ::motionHandle(){
    return m_backend;
}

void CalibrateZ::setMotionHandle(BackEnd *backend)
{
    m_backend = backend;
}

CVCamera* CalibrateZ::camera(){
    return m_camera;
}

void CalibrateZ::setCamera(CVCamera *camera)
{
    m_camera = camera;
}

void CalibrateZ::setRefImage()
{
    // since this function is called only once,
    // I delayed the creation of thread and worker until now.
    if(m_backend == nullptr || m_camera == nullptr) return;
    if(m_calibThread == nullptr) {
        // delay the inialization until now.
        m_calibThread = new QThread(this);
        m_worker = new CalibrateWorker(m_backend->getMotionController(), m_camera);
        m_worker->moveToThread(m_calibThread);

        connect(m_calibThread, SIGNAL(started()), m_worker, SLOT(start()));
        connect(m_calibThread, SIGNAL(finished()), m_worker, SLOT(stop()));

        connect(m_worker, SIGNAL(focusPointFound()), this, SIGNAL(focusPointFound()));

        m_calibThread->start();
    }

     m_worker->setMean(55.566);
    // get reference image from camera and calculate the mean;
//    QVariant image = m_camera->getCvImage();
//    if(image.canConvert<cv::Mat>()) {
//        double mean = this->calMean(image.value<cv::Mat>());
//        m_worker->setMean(mean);
//        qInfo() << "mean value of referece image: " << mean;
//    }
}

double CalibrateZ::calMean(cv::Mat image)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    return cv::mean(gray)[0];
}

double CalibrateZ::calMean(QVariant input) {
    if(input.canConvert<cv::Mat>()) {
        return calMean(input.value<cv::Mat>());
    } else {
        return 9999;
    }
}

void CalibrateZ::startLoop(){
    return;
}
