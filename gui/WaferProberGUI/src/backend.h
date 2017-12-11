#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QQmlEngine>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QMutex>

#include "ControllerBase.h" // from libWaferProb

#include <string>
#include <vector>


#define X_MAX 305  // unit of mm.
#define X_MIN 0
#define Y_MAX 305  // unit of mm.
#define Y_MIN 0
#define Z_MAX 9    // unit of mm. range is: [0, 9] mm
#define Z_MIN 0

class MotionWorker : public QObject {
    Q_OBJECT

public:
    explicit MotionWorker(ControllerBase* ctrl);

    void add_cmd(QString cmd);

public slots:
    void start();
    void stop();

private slots:
    void run();

signals:
    void positionChanged(int axis);
    void commandChanged(QString command);

protected:

   QVector<QString>* cmd_queue;
   ControllerBase* m_ctrl;
   QMutex m_cmdQueueMutex;
   QTimer* m_timer;
};


class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString xyDeviceName READ xyDeviceName WRITE setXyDeviceName NOTIFY xyDeviceNameChanged)

    // set speed
    Q_PROPERTY(float speedX READ getSpeedX WRITE setSpeedX NOTIFY speedXSet)
    Q_PROPERTY(float speedY READ getSpeedY WRITE setSpeedY NOTIFY speedYSet)
    Q_PROPERTY(float speedZ READ getSpeedZ WRITE setSpeedZ NOTIFY speedZSet)

    // z-axis.
    Q_PROPERTY(bool zContact READ zContact WRITE setZContact)
    Q_PROPERTY(float zSep READ zSep WRITE setZSep)
    Q_PROPERTY(bool IsAtContact READ IsAtContact WRITE setIsAtContact)

// functions that can be called by QML
public:
    Q_INVOKABLE int connectDevice();
    Q_INVOKABLE void dismiss();

    Q_INVOKABLE void run_cmd(QString cmd);

//    Q_INVOKABLE void calibrateZ(){
//        m_ctrl->calibrate_Z();
//    }

    // stop motions
    Q_INVOKABLE void stop(){
        worker->stop();
        m_ctrl->stop();
    }

    Q_INVOKABLE float getPosX(){ return m_current_x; emit posXGot(); }
    Q_INVOKABLE float getPosY(){ return m_current_y; emit posYGot(); }
    Q_INVOKABLE float getPosZ(){
        m_current_z = m_ctrl->m_position[2];
        return m_current_z;
        emit posZGot();
    }

    ControllerBase* getMotionController() {
        return m_ctrl;
    }

public:
    explicit BackEnd(QObject *parent = nullptr);

    QString xyDeviceName(){ return m_xyDeviceName; }
    void setXyDeviceName(QString& name) {
        m_xyDeviceName = name;
        emit xyDeviceNameChanged();
    }

    void setSpeedX(float speed_x);
    void setSpeedY(float speed_y);
    void setSpeedZ(float speed_z);

    float getSpeedX() { return m_speed_x; }
    float getSpeedY() { return m_speed_y; }
    float getSpeedZ() { return m_speed_z; }

    // read
    bool zContact(){ return m_z_isContact; }
    void setZContact(bool is_contact) {
        if(is_contact && ! m_z_isContact){
            // move to contact
            m_ctrl->mv_rel(2, m_z_sep);
            m_z_isContact = true;
        } else if (!is_contact && m_z_isContact){
            // move to separate
            m_ctrl->mv_rel(2, -1*m_z_sep);
            m_z_isContact = false;
        } else {
            // don't move
        }
    }

    bool IsAtContact() {return m_z_isContact; }
    void setIsAtContact(bool is_contact) {
        m_z_isContact = is_contact;
    }

    float zSep(){return m_z_sep;}
    void setZSep(float sep){
        m_z_sep = sep;
    }

    void setTestXY(float axis);

public slots:
    void checkPositionInfo(int axis){
        if(axis == 0 || axis == 1) {
            get_pos_xy();
        }
        emit positionChanged(axis);
    }

    void receiveUpdate(QString message) {
        if(message.contains("ENDCHIP")){
            emit chipArrived();
        } else if(message.contains("ENDCALIBRATE")){
            emit srcImageArrived();
        } else if(message.contains("ENDFORCALIBRATEZ")) {
            emit chipArrivedForCalibrateZ();
        } else {
            emit infoUpdated(message);
        }
    }

signals:
    void xyDeviceNameChanged();
    void deviceConnected();

    void positionChanged(int axis);
    void infoUpdated(QString message);

    void chipArrived();
    void srcImageArrived();
    void chipArrivedForCalibrateZ();

    void posXGot(); // X postion returned
    void posYGot(); // Y postion returned
    void posZGot(); // Z postion returned

    void speedXSet();
    void speedYSet();
    void speedZSet();

private:
    QString m_xyDeviceName;
    ControllerBase* m_ctrl;

    float m_current_x;
    float m_current_y;
    float m_current_z;

    float m_z_sep; // distance between needle and chip when in status of separation
    bool m_z_isContact; // tell the program if needle and chip are in contact.

    float m_speed_x;
    float m_speed_y;
    float m_speed_z;

    int unit;
private: // private functions
    bool is_valid_x(float x){
        return x >= X_MIN && x <= X_MAX;
    }
    bool is_valid_y(float y){
        return y >= Y_MIN && y <= Y_MAX;
    }
    bool is_valid_z(float z){
        return z >= Z_MIN && z <= Z_MAX;
    }
    void get_pos_xy();

private:
    QThread* m_motionControlThread;
    MotionWorker* worker;
};

// QML_DECLARE_TYPEINFO(BackEnd, QML_HAS_ATTACHED_PROPERTIES)
#endif // BACKEND_H
