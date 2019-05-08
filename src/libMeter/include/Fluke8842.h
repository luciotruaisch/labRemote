#ifndef FLUKE8842_H
#define FLUKE8842_H

// ####################
// Fluke 8842A Multimeter
// Author: 
// Date: Nov 2018
// Notes: Assuming Prologix GPIB to USB
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SerialCom.h"

enum class FlukeMode {
    VOLTAGEDC,
    VOLTAGEAC,
    CURRENTDC,
    CURRENTAC
};

class Fluke8842 {
    public:
        Fluke8842(std::string dev, unsigned addr);
        ~Fluke8842();

        void init();
        void setSense(enum FlukeMode);

        std::string sense();


    private:
        SerialCom *m_com;
        unsigned m_addr;

        void send(std::string cmd);
        std::string receive(std::string cmd);

        std::chrono::milliseconds m_wait{500};
};

#endif

