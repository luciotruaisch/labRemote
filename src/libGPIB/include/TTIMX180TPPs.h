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
#include "GenericPs.h"

#include "SerialCom.h"

class TTIMX180TPPs : public GenericPs{
    public:
        TTIMX180TPPs(std::string dev, unsigned addr);
        ~TTIMX180TPPs();

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
        unsigned m_channel;

        void send(std::string cmd);
        std::string receive(std::string cmd);
        
        std::chrono::milliseconds m_wait{200};
};

#endif
