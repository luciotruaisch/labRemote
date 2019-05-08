#ifndef GENERICLOAD_H
#define GENERICLOAD_H

#include <string>

class GenericLoad {
    public:
        virtual void setCurrent(double cur) = 0;
        virtual std::string getCurrent() = 0;
        virtual void setVoltage(double volt) = 0;
        virtual std::string getVoltage() = 0;
        virtual void turnOn() = 0;
        virtual void turnOff() = 0;
};

#endif
