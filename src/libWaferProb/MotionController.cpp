#include "MotionController.h"
#include "Helper.h"

#include <stdio.h>

#include <string>
#include <sstream>
#include <vector>

#include <unistd.h>  // for sleep()

using namespace std;
MotionController::MotionController(const char* dn_1):
    ControllerBase()
{
    xy_ctrl = new ControllerZaber(dn_1);
    z_ctrl = new ControllerGalil("192.168.1.30");
    m_position[0] = m_position[1] = m_position[2] = -1;
}

MotionController::~MotionController(){
    disconnect();
}

int MotionController::connect() {
    if(xy_ctrl->connect() != 0){ 
        return 1;
    }
    xy_ctrl->unpark();
    // xy_ctrl->set_home();

    // connect z station
    if(z_ctrl->connect() != 0){
        return 2;
    }

    return 0;
}

int MotionController::disconnect(){
    // before disconnect, park the device.
    xy_ctrl->park();
    xy_ctrl->disconnect();

    z_ctrl->disconnect();
    return 0;
}

int MotionController::set_speed(int axis, float sp){
    if (axis == 0 || axis == 1) { // x-y-axis
        xy_ctrl->set_speed(axis+1, sp);
    } else if (axis == 2) {
        z_ctrl->set_speed(axis, sp);
    } else {
        ;
    }
    return 0;
}

int MotionController::mv_abs(int axis, float value){
    if(axis == 0 || axis == 1){
        xy_ctrl->mv_abs(axis+1, value);
    } else if (axis == 2){
        z_ctrl->mv_abs(axis, value);
    } else {
        ;
    }
    return 0;
}

int MotionController::mv_rel(int axis, float value) {
    if(axis == 0 || axis == 1){
        xy_ctrl->mv_rel(axis+1, value);
    } else if (axis == 2) {
        z_ctrl->mv_rel(axis, value);
    } else {}
    return 0;
}

int MotionController::stop(){
    xy_ctrl->stop();
    z_ctrl->stop();
    return 0;
}

int MotionController::get_position(){
    get_pos_xy();
    get_pos_z();
    return 0;
}

int MotionController::get_pos_xy(){
    xy_ctrl->get_position();
    m_position[0] = xy_ctrl->m_position[0];
    m_position[1] = xy_ctrl->m_position[1];
    return 0;
}

int MotionController::get_pos_z(){
    z_ctrl->get_position();
    m_position[2] = z_ctrl->m_position[2];
    return 0;
}


int MotionController::get_position(int axis){
    get_position();
    return m_position[axis];
}

int MotionController::write(int axis, const string& cmd) 
{
    if(axis == 0 || axis == 1){
        xy_ctrl->write(cmd);
    } else if(axis == 2){
        z_ctrl->write(cmd);
    } else {}
    return 0;
}

int MotionController::set_home(){
    return xy_ctrl->set_home();
}

int MotionController::set_center(){
    return xy_ctrl->set_center();
}

int MotionController::run_cmd(const string& cmd) 
{
    int axis = -1;
    float unit_scale = 1000.;
    if(cmd.empty()){
        return axis;
    }
    vector<string> raw_items;
    WaferProb::tokenizeString(cmd, ' ', raw_items);

    vector<string> items;
    // convert commands to uppercase
    for(auto& item: raw_items){
        items.push_back(WaferProb::toUpper(item)); 
    }

    const string& action(items[0]);
    // Check each case..
    if (action == "MA")
    {
        if(items.size() != 3){
            printf("argument of MA is wrong\n"
                    "MA X/Y/Z 10\n");
            return axis;
        }
        axis = WaferProb::axis_number(items[1]);
        this->mv_abs(axis, unit_scale * atof(items[2].c_str()));
    } else if (action == "MR")
    {
        if(items.size() != 3){
            printf("argument of MR is wrong\n"
                    "MR X/Y/Z 10\n");
            return axis;
        }
        axis = WaferProb::axis_number(items[1]);
        if (axis == 2) {
            this->mv_rel(axis, atof(items[2].c_str()));
        } else {
            this->mv_rel(axis, unit_scale * atof(items[2].c_str()));
        }
    } else if (action == "SH")
    {
        this->set_home();
    } else if (action == "SM")
    {
        this->set_center();
    } else if (action == "SP")
    {
        if (items.size() != 3){
            printf("argument of SP is wrong\n"
                    "SP X/Y/Z 10000\n");
            return axis;
        }
        axis = WaferProb::axis_number(items[1]);
        this->set_speed(axis, unit_scale * atof(items[2].c_str()));
    }else if (action == "TEST"){
        vector<int> steps{20, 46, 73, 100, 126, 152, 179, 206, 226};
        if (items.size() != 2){
            printf("argument of TEST is wrong\n"
                    "TEST X/Y \n");
            return axis;
        }
        axis = WaferProb::axis_number(items[1]);
        for(int step : steps){
            this->mv_abs(axis, unit_scale * step);
            sleep(10);
        }
    }else if (action == "SCAN"){
        if(items.size() != 2) {
            printf("argument of SCAN is wrong\n"
                    "SCAN X/Y \n");
            return axis;
        }
        axis = WaferProb::axis_number(items[1]);
        // protect from axis being z-axis
        if(axis == 0) {
            this->mv_abs(axis, 0);
            sleep(1);
        }
        if(axis == 1) {
            this->mv_abs(axis, 305);
            sleep(1);
        }
    } else {
        printf("%s not supported yet!\n", action.c_str());
        // print_cmd();
    }
    return axis;
}
