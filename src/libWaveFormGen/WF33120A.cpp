#include "WF33120A.h"
#include "Logger.h"


WF33120A::WF33120A(std::string dev, unsigned addr) {
    m_com = new SerialCom(dev);
    m_addr = addr;
    m_com->write("++auto 0\n\r");
}

WF33120A::~WF33120A() {
    delete m_com;
}

void WF33120A::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string WF33120A::receive(std::string cmd) {
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

void WF33120A::init() {
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Initialising: ";
    this->send("*RST");
    sleep(1);
    this->send("FUNC:SHAP SQU"); 
    sleep(1);
    this->send("VOLT 1.2");
    sleep(1);
    this->send("VOLT:OFFS 0.6"); 
}

void WF33120A::setFreq(double value) { //Frequency  Hz
    sleep(1);
    this->send("FREQ " + std::to_string(value));
}

void WF33120A::setVolt(double value) { //Amplitude  V
    sleep(1);
    this->send("VOLT " + std::to_string(value));
}

void WF33120A::setVoltOffset(double value) { //Offset  V
    sleep(1);
    this->send("VOLT:OFFS " + std::to_string(value));
}

std::string WF33120A::getVolt() {
     std::string result = this->receive("VOLT?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

std::string WF33120A::getVoltUnit() {
     std::string result = this->receive("VOLT:UNIT?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

std::string WF33120A::getVoltOffset() {
     std::string result = this->receive("VOLT:OFFS?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

std::string WF33120A::getVoltOffsetUnit() {
     std::string result = this->receive("VOLT:OFFS:UNIT?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

std::string WF33120A::getFreq() {
     std::string result = this->receive("FREQ?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

std::string WF33120A::getFunc() {
     std::string result = this->receive("FUNC?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

