#ifndef SERIALCOM_H
#define SERIALCOM_H

// ####################
// Serial Communication Class
// Author: Timon Heim
// Date: Feb 2017
// Notes: blocking posix implementation
// ###################

#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>      
#include <errno.h>

class SerialCom {
	public:
		SerialCom(std::string deviceName);
		SerialCom(std::string deviceName, speed_t baud);
		~SerialCom();

		int write(char *buf, size_t length);
		int read(char *buf, size_t length);
		int write(const std::string buf);
		int read(std::string &buf);
		int flush();
            
        bool is_open();
	private:	
		void init();
		void config();
		
		int m_dev;
		std::string m_deviceName;
		speed_t m_baudrate;
		struct termios tty;
		struct termios tty_old;
        bool m_good;
        const unsigned MAX_READ = 4096;
};

#endif
