#ifndef KEITHLEY24XX_H
#define KEITHLEY24XX_H

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
#include <iomanip>
#include <sstream>

#include "SerialCom.h"

enum class KeithleyMode {
    VOLTAGE,
    CURRENT
};

class Keithley24XX {
    public:
        Keithley24XX(std::string dev, unsigned addr);
        ~Keithley24XX();

        void init();
        void turnOn();
        void turnOff();
        bool isOn();
        void setSource(enum KeithleyMode, double range, double value);
        void setSense(enum KeithleyMode, double range, double protection);
        std::string sense(enum KeithleyMode);
        void setCompl(enum KeithleyMode, double protection);


    private:
        SerialCom *m_com;
        unsigned m_addr;

        void send(std::string cmd);
        std::string receive(std::string cmd);

        std::chrono::milliseconds m_wait{200};
        
        template <typename T>
            std::string to_string_with_precision(const T a_value, const int n = 6)
            {
                std::ostringstream out;
                out << std::setprecision(n) << a_value;
                return out.str();

            }
};

#endif

