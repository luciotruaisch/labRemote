#ifndef GENERICPS_H
#define GENERICPS_H

#include <string>

class GenericPs {
    public:
        virtual void setCurrent(double cur) = 0;
        virtual std::string getCurrent() = 0;
        virtual void setVoltage(double volt) = 0;
        virtual std::string getVoltage() = 0;
};

#endif
