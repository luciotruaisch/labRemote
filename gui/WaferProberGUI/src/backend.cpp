#include "backend.h"
#include "Helper.h" // from libWaferProb
#include "MotionController.h"

#include <QDebug>
#include <QThread> // for sleep function.

#include <vector>
#include <string>

using namespace std;
MotionWorker::MotionWorker(ControllerBase* ctrl)
{
    cmd_queue = new QVector<QString>();
    this->m_ctrl = ctrl;

    // Timer will send a signal of "timeout" in a constant time interval!
    // so the run() will be processed every time a "timeout" is received.
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(run()));
}

void MotionWorker::start(){
    m_timer->start(1500);
}

void MotionWorker::stop() {
    m_cmdQueueMutex.lock();
    cmd_queue->clear();
    m_cmdQueueMutex.unlock();
}

void MotionWorker::add_cmd(QString cmd)
{
    m_cmdQueueMutex.lock();
    cmd_queue->push_back(cmd);
    m_cmdQueueMutex.unlock();
    // qInfo() <<" size of commands: " << cmd_queue->size();
}

void MotionWorker::run()
{

    if(!cmd_queue->empty()){

        m_cmdQueueMutex.lock();

        QString current_cmd = cmd_queue->first();

        cmd_queue->pop_front();
        // qInfo() <<" size of commands after run: " << cmd_queue->size();
        m_cmdQueueMutex.unlock();

        if(!current_cmd.contains("END")) {
            //every command that has "END" is to emit a signal.
            int axis_changed = m_ctrl->run_cmd(current_cmd.toLatin1().data());

            // signal the command that is processed for records
            // and notify main program that a axis is changed.
            emit positionChanged(axis_changed);
        } else {
            QThread::sleep(1);
        }
        emit commandChanged(current_cmd);
    }
}


BackEnd::BackEnd(QObject *parent) : QObject(parent)
{

    m_ctrl = 0;
    m_current_x = m_current_y = m_current_z = -1.0;
    m_z_sep = 0.700; // unit of mm.
    m_z_isContact = false;

}

int BackEnd::connectDevice()
{
    if(m_ctrl == 0) {

        const char* deviceName = m_xyDeviceName.toLatin1().data();
        // One can changes to any other motion controller that
        // is derived from ControllerBase.
        m_ctrl = new MotionController(deviceName);
    }
    int status = m_ctrl->connect();
    if(status == 0){
        get_pos_xy();
    }

    emit deviceConnected();

    // thread to control motion...
    m_motionControlThread = new QThread(this);
    worker = new MotionWorker(m_ctrl);
    worker->moveToThread(m_motionControlThread);
    connect(m_motionControlThread, SIGNAL(started()), worker, SLOT(start()));
    connect(m_motionControlThread, SIGNAL(finished()), worker, SLOT(stop()));

    connect(worker, SIGNAL(positionChanged(int)), this, SLOT(checkPositionInfo(int)));
    connect(worker, SIGNAL(commandChanged(QString)), this, SLOT(receiveUpdate(QString)));
    m_motionControlThread->start();

    return status;
}

void BackEnd::dismiss(){
    if (m_ctrl != 0) m_ctrl->disconnect();

    m_motionControlThread->quit();
}

void BackEnd::get_pos_xy(){
    m_current_x = m_ctrl->m_position[0];
    m_current_y = m_ctrl->m_position[1];
}

void BackEnd::setSpeedX(float speed_x){
    m_speed_x = speed_x;
    m_ctrl->set_speed(0, m_speed_x);
    emit speedXSet();
}

void BackEnd::setSpeedY(float speed_y){
    m_speed_y = speed_y;
    m_ctrl->set_speed(1, m_speed_y);
    emit speedYSet();
}

void BackEnd::setSpeedZ(float speed_z){
    m_speed_z = speed_z;
    m_ctrl->set_speed(2, m_speed_z);
    emit speedZSet();
}

void BackEnd::setTestXY(float axis){
    vector<int> steps{20, 46, 73, 100, 126, 152, 179, 206, 226};
    for(int step: steps){
        m_ctrl->mv_abs(axis, step);
        QThread::sleep(6);
    }
}

void BackEnd::run_cmd(QString cmd) {
    worker->add_cmd(cmd);
}
