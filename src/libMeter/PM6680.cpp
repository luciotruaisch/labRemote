#include "PM6680.h"
#include "Logger.h"


PM6680::PM6680(std::string dev, unsigned addr) {
    m_com = new SerialCom(dev);
    m_addr = addr;
    m_com->write("++auto 0\n\r");
}

PM6680::~PM6680() {
    delete m_com;
}

void PM6680::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string PM6680::receive(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    sleep(1);
    m_com->write("++read eoi\n\r");
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
    std::string buf;
    m_com->read(buf);
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Received: " << buf;
    return buf;
}

void PM6680::init() {
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Initialising: ";
    logger(logDEBUG2) << this->receive("*IDN?");
    this->send("*RST");
    sleep(1);
    this->send("INPUT:LEVEL:AUTO ON");
    sleep(1);
    this->send("INP:COUP DC");
    sleep(1);
    this->send("INP:IMP 1000000");
    sleep(1);
    this->send("INP:SLOP POS");
    sleep(1);
    this->send("INP:FILT ON");
    sleep(1);
   this->send("AVER:STATE ON");
    
}

std::string PM6680::getFrequency() {
    //std::string result=this->receive("MEAS:ARRay:FREQ? (10)");
    std::string result=this->receive("MEAS:FREQ?");
    //std::string result=this->receive("MEAS:FREQ? (10)");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
    return result;
}

