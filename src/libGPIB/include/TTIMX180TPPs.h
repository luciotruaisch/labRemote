#ifndef TTIMX180TPPS_H
#define TTIMX180TPPS_H

// ####################
// TTi Power supply
// Author: Neha Santpur
// Date: Sep 2018
// Notes: Assuming Prologix GPIB to USB
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SerialCom.h"

class TTIMX180TPPs {
    public:
        TTIMX180TPPs(std::string dev, unsigned addr);
        ~TTIMX180TPPs();

        void init();
        void setCh(unsigned ch);
        void setRange(unsigned number,unsigned range);
        void setVoltage(unsigned number,double volt);
        std::string getVoltage(unsigned number);
        void setCurrent(unsigned number,double cur);
        std::string getCurrent(unsigned number);
        void turnOn(unsigned number);
        void turnOff(unsigned number);

    private:
        SerialCom *m_com;
        unsigned m_addr;

        void send(std::string cmd);
        std::string receive(std::string cmd);
        
        std::chrono::milliseconds m_wait{200};
};

#endif
