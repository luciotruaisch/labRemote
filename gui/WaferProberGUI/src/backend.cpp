#include "backend.h"
#include "Helper.h" // from libWaferProb

#include <QDebug>
#include <QThread> // for sleep function.

#include <vector>
#include <string>

using namespace std;
MotionWorker::MotionWorker(MotionController* ctrl)
{
    cmd_queue.clear();
    this->backend = ctrl;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(run()));
}

void MotionWorker::start(){
    m_timer->start(1000);
}

void MotionWorker::stop() {
    m_timer->stop();
    cmd_queue.clear();
}

void MotionWorker::run_cmd(QString cmd)
{
    cmd_queue.push_back(cmd);
}

void MotionWorker::run()
{
    if(!cmd_queue.empty()){
        QString& current_cmd = cmd_queue.first();
        backend->run_cmd(current_cmd.toLatin1().data());
        cmd_queue.pop_front();
    }
}


BackEnd::BackEnd(QObject *parent) : QObject(parent)
{

    m_ctrl = 0;
    m_current_x = m_current_y = m_current_z = -1.0;
    unit = 1000;
    m_z_sep = 0.700; // unit of mm.
    m_z_isContact = false;

    // thread to control motion...
    m_motionControlThread = new QThread(this);
    MotionWorker* worker = new MotionWorker(m_ctrl);
    worker->moveToThread(m_motionControlThread);
    connect(m_motionControlThread, SIGNAL(started()), worker, SLOT(start()));
    connect(m_motionControlThread, SIGNAL(finished()), worker, SLOT(stop()));
//    connect(worker, SIGNAL(finished()), m_motionControlThread, SLOT(quit()));
    m_motionControlThread->start();

}

int BackEnd::connectDevice()
{
    if(m_ctrl == 0) {
        const char* deviceName = m_xyDeviceName.toLatin1().data();
        m_ctrl = new MotionController(deviceName);
    }
    int status = m_ctrl->connect();
    if(status == 0){
        get_pos_xy();
    }
    emit deviceConnected();
    return status;
}

bool BackEnd::dismiss(){
    if (m_ctrl != 0) m_ctrl->disconnect();

    m_motionControlThread->quit();

    return true;
}

void BackEnd::setAbs_x(float x){
    if(x != m_current_x && is_valid_x(x)){
        m_abs_x = x;
        m_ctrl->mv_abs(0, m_abs_x*unit);
        emit posXChanged();
    }
}

void BackEnd::setAbs_y(float y){
    if(y != m_current_y && is_valid_y(y)){
        m_abs_y = y;
        m_ctrl->mv_abs(1, m_abs_y*unit);
        emit posYChanged();
    }
}

void BackEnd::setRel_x(float x){
    if(is_valid_x(x+m_current_x)){
        m_rel_x = x;
        m_ctrl->mv_rel(0, m_rel_x*unit);
        emit posXChanged();
    }
}

void BackEnd::setRel_y(float y){
    if(is_valid_y(y+m_current_y)){
        m_rel_y = y;
        m_ctrl->mv_rel(1, m_rel_y*unit);
        emit posYChanged();
    }
}

void BackEnd::setRel_z(float z){
    // z is in unit of mm.
    m_rel_z = z;
    /**
    float abs_z = z + m_current_z;
    if(abs_z > Z_MAX){
        printf("will only go to maximum\n");
        m_ctrl->mv_abs(2, Z_MAX);
    } else {
        m_ctrl->mv_rel(2, m_rel_z);
    }
    **/
    m_ctrl->mv_rel(2, m_rel_z);
    m_ctrl->get_pos_z();
    m_current_z = m_ctrl->m_position[2];
    emit posZChanged();
}

bool BackEnd::runSH(){
    m_ctrl->set_home();
    // update current position

    emit posXYChanged();
    return m_runSH;
}

bool BackEnd::runSM(){
    m_ctrl->set_center();

    emit posXYChanged();
    return m_runSM;
}

void BackEnd::get_pos_xy(){
    m_ctrl->get_pos_xy();
    m_current_x = m_ctrl->m_position[0];
    m_current_y = m_ctrl->m_position[1];
}

void BackEnd::setSpeedX(float speed_x){
    m_speed_x = speed_x;
    m_ctrl->set_speed(0, m_speed_x*unit);
    emit speedXSet();
}

void BackEnd::setSpeedY(float speed_y){
    m_speed_y = speed_y;
    m_ctrl->set_speed(1, m_speed_y*unit);
    emit speedYSet();
}

void BackEnd::setSpeedZ(float speed_z){
    m_speed_z = speed_z;
    m_ctrl->set_speed(2, m_speed_z);
    emit speedZSet();
}

bool BackEnd::zTop(){
    m_ctrl->mv_abs(2, Z_MAX);
    m_ctrl->get_pos_z();
    m_current_z = m_ctrl->m_position[2];
    return true;
}

bool BackEnd::zBottom(){
    m_ctrl->mv_abs(2, 0.);
    m_ctrl->get_pos_z();
    m_current_z = m_ctrl->m_position[2];
    return true;
}

bool BackEnd::zMid(){
    m_ctrl->mv_abs(2, 7.0945);
    m_ctrl->get_pos_z();
    m_current_z = m_ctrl->m_position[2];
    return true;
}

void BackEnd::scanX(int times){
    if(times < 0) { times = 1; }
    m_scan_x = times;

    for(int i = 0; i < times; i++){
        m_ctrl->mv_abs(0, 0);
        QThread::sleep(3);
        m_ctrl->mv_abs(0, X_MAX*unit);
        QThread::sleep(3);
    }
}

void BackEnd::scanY(int times){
    if(times < 0) { times = 1; }
    m_scan_y = times;

    for(int i = 0; i < times; i++){
        m_ctrl->mv_abs(1, 0);
        QThread::sleep(3);
        m_ctrl->mv_abs(1, Y_MAX*unit);
        QThread::sleep(3);
    }
}

void BackEnd::setTestXY(float axis){
    vector<int> steps{20, 46, 73, 100, 126, 152, 179, 206, 226};
    for(int step: steps){
        m_ctrl->mv_abs(axis, unit*step);
        QThread::sleep(6);
    }
}
