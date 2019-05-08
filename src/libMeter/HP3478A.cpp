#include "HP3478A.h"
#include "Logger.h"

//Commands predate SCPI. See manual for more details.


HP3478A::HP3478A(std::string dev, unsigned addr) {
	m_com = new SerialCom(dev, B115200);
	m_addr = addr;
	m_com->write("++auto 0\n\r");
}


HP3478A::~HP3478A() {
	delete m_com;
}

// TODO send/receive should be in prologix class
void HP3478A::send(std::string cmd) {
	m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
	logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
	cmd += "\r\n";
	m_com->write(cmd);
	std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}


std::string HP3478A::receive(std::string cmd) {
	m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
	logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
	cmd += "\r\n";
	m_com->write(cmd);
	m_com->write("++read eoi\n\r");
	std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
	std::string buf;
	m_com->read(buf);
	logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Received: " << buf;
	return buf;
}


//All these settings uses digit_mode="N5" ( 5 1/2 digit mode; best noise rejection, but slowest reading rate).
std::string HP3478A::setSense(enum HPMode mode) {
	switch (mode) {
		case HPMode::SETTING:  //currently not working
			this->send("\"S\"");
			this->receive("S1"); // S0/S1 for rear/front panel
			this->send("DISP S1");
			//this->send("PRINT");
			this->send("END");
			break;
		case HPMode::DC_VOLTAGE:
			this->send("++auto 0\n\r");
			this->send("\"F1RAZ1N5\"");
			this->send("++auto 1\n\r");
			return this->receive("++read 10\n\r");
			//std::cout << this->receive("++read 10\n\r");
			//this->send("TRIGGER 7");
			//this->send("ENTER 704; I10");
			//this->send("DISP I10");
			//this->receive("PRINT I10");
			break;
		case HPMode::DC_CURRENT:
			this->send("++auto 0\n\r");
			this->send("\"F5RAZ1N5\"");
			this->send("++auto 1\n\r");
			return this->receive("++read 10\n\r");
			//std::cout << this->receive("++read 10\n\r");
			//this->send("ENTER 704; B1");
			//this->send("DISP A3");
			//this->receive("PRINT A3");
			break;
		case HPMode::AC_VOLTAGE:
			this->send("++auto 0\n\r");
			this->send("F2RAZ1N5\"");
			this->receive("++auto 1\n\r");
			return this->receive("++read 10\n\r");
			break;
		case HPMode::AC_CURRENT:
			this->send("++auto 0\n\r");
			this->send("\"F6RAZ1N5\"");
			this->receive("++auto 1\n\r");
			return this->receive("++read 10\n\r");
			break;
		default:
			logger(logERROR) << __PRETTY_FUNCTION__ << " : Unknown mode!";
			break;
	}
}




