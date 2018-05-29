#ifndef HP3478A_H
#define HP3478A_H

// ####################
// HP Source Meter
// Author: Timon Heim
// Date: Aug 2017
// Notes: Assuming Prologix GPIB to USB. Original file for Keithley power supplies; edited by Charilou Labitan to use for HP multimeters.
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SerialCom.h"

enum class HPMode {
	SETTING,
	DC_VOLTAGE,
	DC_CURRENT,
	AC_VOLTAGE,
	AC_CURRENT,

};

class HP3478A {
	public:
		HP3478A(std::string dev, unsigned addr);
		~HP3478A();

		std::string setSense(enum HPMode);

		void send(std::string cmd);
		std::string receive(std::string cmd);


	private:
		SerialCom *m_com;
		unsigned m_addr;


		std::chrono::milliseconds m_wait{600}; //200
};

#endif

