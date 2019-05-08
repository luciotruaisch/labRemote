#ifndef Tektronix654C_H
#define Tektronix654C_H

// ####################
// Tektronix 654C and others
// Author: Titus Amza
// Date: Jul 2018
// Notes: Assuming Prologix GPIB to USB
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SerialCom.h"

class Tektronix654C {
    public:
        Tektronix654C(std::string dev, unsigned addr);
        ~Tektronix654C();

        std::string init();
	std::string curve();
        std::string read();
	std::string read1char(int n);
        void setCh(std::string ch);
	void setRef(std::string ref);
	void setData(std::string data);
	void saveWaveToRef(std::string ch, std::string ref); 
	void hardCopyPort();
	void hardCopyFormat();
	std::string verticalPos(std::string ch);
	std::string voltDiv(std::string ch);
	std::string timeBase();
	void rstWave();

    private:
        SerialCom *m_com;
        unsigned m_addr;

        void send(std::string cmd);
        std::string receive(std::string cmd);
        
        std::chrono::milliseconds m_wait{300};
};

#endif
