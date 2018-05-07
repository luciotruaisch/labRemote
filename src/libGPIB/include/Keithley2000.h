#ifndef KEITHLEY2000_H
#define KEITHLEY2000_H

// ####################
// Keithley Source Meter
// Author: Timon Heim
// Date: Aug 2017
// Notes: Assuming Prologix GPIB to USB
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SerialCom.h"

enum class KeithleyMode {
    VOLTAGE,
    CURRENT
};

class Keithley2000 {
    public:
        Keithley2000(std::string dev, unsigned addr);
        ~Keithley2000();

        void init();
        void setSense(enum KeithleyMode);

	std::string readChannel(std::string channel);
        std::string sense();


    private:
        SerialCom *m_com;
        unsigned m_addr;

        void send(std::string cmd);
        std::string receive(std::string cmd);

        std::chrono::milliseconds m_wait{200};
};

#endif

