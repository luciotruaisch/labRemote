#ifndef TTILD400P_H
#define TTILD400P_H

//###############
// TTILD400 PS
// April 2019
// Prologix GPIB Comm
//###############

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "GenericLoad.h"

#include "SerialCom.h"

class TTILD400P : public GenericLoad{
    public:
        TTILD400P(std::string dev, unsigned addr);
        ~TTILD400P();

        void init();
	void setVoltage(double volt);
        std::string getVoltage();
        void setCurrent(double cur);
        std::string getCurrent();
        void turnOn();
        void turnOff();

	void setCurrentLim(double cur);	
	void setVoltageLim(double vol);

	void setModeCC();
        void setModeCV();
        void setModeCW();
        void setModeCR();
	std::string getMode();
	
    private:
        SerialCom *m_com;
        unsigned m_addr;
        unsigned m_channel;

        void send(std::string cmd);
 
	std::string receive(std::string cmd);
        
        std::chrono::milliseconds m_wait{500};
};

#endif
