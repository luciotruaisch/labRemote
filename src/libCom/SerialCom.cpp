#include "SerialCom.h"
#include "Logger.h"

#include <cerrno>
#include <cstring>

loglevel_e loglevel;

SerialCom::SerialCom(std::string name) {
	m_dev = 0;
    m_deviceName = name;
	m_baudrate = B19200;
    m_good = true;
    this->init(); 
}

SerialCom::SerialCom(std::string name, speed_t baud) {
	m_dev = 0;
    m_deviceName = name;
	m_baudrate = baud;
    m_good = true;
    this->init(); 
}

SerialCom::~SerialCom() {
	if(m_good) close(m_dev);
}

void SerialCom::init() {
	m_dev = open(m_deviceName.c_str(), O_RDWR | O_NOCTTY);
	this->config();
}

void SerialCom::config() {
    logger(logDEBUG3) << __PRETTY_FUNCTION__;
    
    //Asynchronous read
	fcntl(m_dev, F_SETFL, O_NONBLOCK);
    
	if (tcgetattr(m_dev, &tty)){
		logger(logERROR) << __PRETTY_FUNCTION__ << " -> Could not get tty attributes!";
        m_good = false;
	}

	tty_old = tty;

	cfsetospeed(&tty, m_baudrate);
	cfsetispeed(&tty, m_baudrate);

	tty.c_cflag &= ~PARENB; 	// no parity
	tty.c_cflag &= ~CSTOPB; 	// one stop bit
	tty.c_cflag &= ~CRTSCTS; 	// no flow control
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8; 		// 8 databits
	tty.c_cflag |= CREAD | CLOCAL;	// turn on READ & ignore ctrl lines

	cfmakeraw(&tty);
	tty.c_lflag &= ~(ICANON|ECHO);	// Non-canonical mode, no echo

	tcflush(m_dev, TCIFLUSH);		// Empty buffers

	if (tcsetattr(m_dev, TCSANOW, &tty)){
		logger(logERROR) << __PRETTY_FUNCTION__ << " -> Could not set tty attributes!";
        m_good = false;
	}
}

int SerialCom::flush() {
    return tcflush(m_dev, TCIFLUSH);
}

int SerialCom::write(char *buf, size_t length) {
    return ::write(m_dev, buf, length);
}

int SerialCom::read(char *buf, size_t length) {
    return ::read(m_dev, buf, length);
}

int SerialCom::write(std::string buf) {
    return ::write(m_dev, buf.c_str(), buf.size());
}

int SerialCom::read(std::string &buf) 
{
  char *tmp = new char[MAX_READ];
  int n_read = ::read(m_dev, tmp, MAX_READ);
  if(n_read>=0)
    buf = std::string(tmp, n_read);
  else
    logger(logERROR) << std::strerror(errno);

  return n_read;
}

bool SerialCom::is_open() {
    return m_good;
}

