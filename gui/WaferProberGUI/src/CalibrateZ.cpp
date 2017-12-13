#include "CalibrateZ.h"
#include "opencv2/core.hpp"

#include <QDebug>


Q_DECLARE_METATYPE(cv::Mat)

CalibrateWorker::CalibrateWorker(ControllerBase *mc, CVCamera *camera):
    m_ctrl(mc),
    m_camera(camera)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(run()));

    m_running = false;
}

void CalibrateWorker::start(){
    m_timer->start(1500); // in 1 second
}

void CalibrateWorker::stop(){
    m_running = false;
}

void CalibrateWorker::run()
{
    if(m_running){
        // start to move chunk up and down to compare to mean value.
        m_ctrl->set_speed(2, 0.040);

        const double MAX_DISTANCE = 0.300;
        const int Max_TRIES = 150;
        const int MAX_FLIPS = 10;
        double step = 0.02;

        bool foundFocus = true;

        double direction = 1; // 1 for increase; -1 for decrease

        double offset = 0.;

        double currentFeature = CalibrateZ::calFeature(m_camera->getCvImage());
        double previousFeature = currentFeature;
        double accumulateMovement = 0;
        int ntry = 1;
        int nFlip = 0;

        // get current absolute position in Z.
        m_ctrl->get_position();
        double absZ = m_ctrl->m_position[2];
        double maxAbsZ = absZ;
        double maxFeature = currentFeature;

        while(m_running && ntry < Max_TRIES) {
            // move to a new position and calculate the feature
            // record the position that yields maximum value of feature.
            // exit when number of Flips reaches or maximum tries (Max_TRIES) reaches.
            double moveZ = (step+offset)*direction;
            accumulateMovement += moveZ;
            if(accumulateMovement > MAX_DISTANCE) {
                qInfo()<<"moved more than "<< MAX_DISTANCE << " um!";
                foundFocus = false;
                break;
            }
            absZ += moveZ;
            m_ctrl->mv_abs(2, absZ);
            currentFeature = CalibrateZ::calFeature(m_camera->getCvImage());

             m_ctrl->get_position();
             qInfo() << "Z: " << absZ << "("<< m_ctrl->m_position[2] << "); mean: " << currentFeature<<"; moved: " << accumulateMovement;
            if(currentFeature > maxFeature) {
                maxFeature = currentFeature;
                maxAbsZ = absZ;
            }
            if(currentFeature < previousFeature)
            {
                // current-feature gets smaller, so change direction and move with a smaller step.
                direction *= -1; // change direction
                offset = step;
                nFlip += 1;
                step *= 0.5;
                // increase the number to get more presion.
                if(nFlip > MAX_FLIPS || step < 0.001) break;
            } else {
                offset = 0;
            }
            previousFeature = currentFeature;
            ntry ++;
        }
//        // m_ctrl->mv_abs(2, minAbsZ);
        qInfo()<<"Ending point: " << maxAbsZ << " " << maxFeature;
//        m_ctrl->get_pos_z();
//        qInfo()<<"New Position: " << m_ctrl->m_position[2];
        m_ctrl->mv_abs(2, maxAbsZ);
        m_ctrl->get_position();
        qInfo()<<"New Position: " << m_ctrl->m_position[2];
        if(foundFocus) emit focusPointFound(maxAbsZ);
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

void CalibrateZ::start() {
    // since this function is called only once,
    // I delayed the creation of thread and worker until now.
    if(m_backend == nullptr || m_camera == nullptr) {
        qInfo() << "no backend or no camera" << endl;
        return;
    }
    if(m_calibThread == nullptr) {
        // delay the inialization until now.
        m_calibThread = new QThread(this);
        m_worker = new CalibrateWorker(m_backend->getMotionController(), m_camera);
        m_worker->moveToThread(m_calibThread);

        connect(m_calibThread, SIGNAL(started()), m_worker, SLOT(start()));
        connect(m_calibThread, SIGNAL(finished()), m_worker, SLOT(stop()));

//        connect(m_worker, SIGNAL(focusPointFound(double)), this, SLOT(receiveFoundedFocus(double)));
        connect(m_worker, SIGNAL(focusPointFound(double)), this, SIGNAL(focusFound()));
        m_calibThread->start();
    }

    if(m_worker) {
        m_worker->startRun();
    }
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

double CalibrateZ::calFeature(cv::Mat image)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Scalar mean, stdv;
    cv::meanStdDev(gray, mean, stdv);
    return stdv[0];
}

double CalibrateZ::calFeature(QVariant input) {
    if(input.canConvert<cv::Mat>()) {
        return calFeature(input.value<cv::Mat>());
    } else {
        return 9999;
    }
}
