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
//        m_ctrl->set_speed(2, 0.050);
//        double step = 0.01;
//        double max_try = 0.300;
        emit focusPointFound();
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

    // get reference image from camera and calculate the mean;
    QVariant image = m_camera->getCvImage();
    if(image.canConvert<cv::Mat>()) {
        double mean = this->calMean(image.value<cv::Mat>());
        m_worker->setMean(mean);
        qInfo() << "mean value of referece image: " << mean;
    }
}

double CalibrateZ::calMean(cv::Mat image)
{
    return cv::mean(image)[0];
}

void CalibrateZ::startLoop(){
    return;
}
