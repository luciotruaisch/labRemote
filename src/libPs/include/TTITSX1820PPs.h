#ifndef TTITSX1820PPS_H
#define TTITSX1820PPS_H

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
#include "GenericPs.h"

class TTITSX1820PPs : public GenericPs {
    public:
        TTITSX1820PPs(std::string dev, unsigned addr);
        ~TTITSX1820PPs();

        void init();
        void setCh(unsigned ch);
        void setRange(unsigned range);
        void setVoltage(double volt);
        std::string getVoltage();
        void setCurrent(double cur);
        std::string getCurrent();
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
