#ifndef WF33120A_H
#define WF33120A_H

// ####################
// hp 15MHz function / arbitrary waveform generator 
// Author: Peilian Liu
// Date: May 2019
// Notes: Assuming Prologix GPIB to USB
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SerialCom.h"

class WF33120A {
    public:
        WF33120A(std::string dev, unsigned addr);
        ~WF33120A();

        void init();
        void setFreq(double value);
        void setVolt(double value);
        void setVoltOffset(double value);
        std::string getVolt();
        std::string getVoltUnit();
        std::string getVoltOffset();
        std::string getVoltOffsetUnit();
        std::string getFreq();
        std::string getFunc();
        void turnOn();
        void turnOff();

    private:
        SerialCom *m_com;
        unsigned m_addr;

        void send(std::string cmd);
        std::string receive(std::string cmd);
 
        std::chrono::milliseconds m_wait{200};
};

#endif
