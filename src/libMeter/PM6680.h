#ifndef PM6680_H
#define PM6680_H

// ####################
// PM 6680 high resolution programmable timer/counter 225MHz  
// Author: Peilian Liu
// Date: May 2019
// Notes: Assuming Prologix GPIB to USB
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SerialCom.h"

class PM6680 {
    public:
        PM6680(std::string dev, unsigned addr);
        ~PM6680();

        void init();
        std::string getFrequency();
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
