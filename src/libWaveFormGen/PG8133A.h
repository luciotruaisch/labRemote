#ifndef PG8133A_H
#define PG8133A_H

// ####################
// Agilent 8133A 3GHz Pulse Generator 
// Author: Peilian Liu
// Date: May 2019
// Notes: Assuming Prologix GPIB to USB
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SerialCom.h"

class PG8133A {
    public:
        PG8133A(std::string dev, unsigned addr);
        ~PG8133A();

        void init();
        void setFreq(double value);
        void setVoltHigh(double value);
        void setVoltLow(double value);
        void setOutPut(double value);
        std::string getOutPen(); 
        std::string getVoltHigh();
        std::string getVoltLow();
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
