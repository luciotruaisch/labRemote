#include "PG8133A.h"
#include "Logger.h"


PG8133A::PG8133A(std::string dev, unsigned addr) {
    m_com = new SerialCom(dev);
    m_addr = addr;
    m_com->write("++auto 0\n\r");
}

PG8133A::~PG8133A() {
    delete m_com;
}

void PG8133A::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string PG8133A::receive(std::string cmd) {
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

void PG8133A::init() {
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Initialising: ";
    this->send("*RST");
    sleep(1);
    this->send("FUNC:SHAP SQU"); 
}

void PG8133A::setFreq(double value) {
    sleep(1);
    this->send("FREQ " + std::to_string(value));
}

void PG8133A::setVoltHigh(double value) {
    sleep(1);
    this->send("VOLT1:HIGH " + std::to_string(value));
}

void PG8133A::setVoltLow(double value) {
    sleep(1);
    this->send("VOLT1:LOW " + std::to_string(value));
}

void PG8133A::setOutPut(double value) {
    sleep(1);
    this->send("OUTP1 " + std::to_string(value));
}

std::string PG8133A::getVoltHigh() {
     std::string result = this->receive("VOLT1:HIGH?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

std::string PG8133A::getVoltLow() {
     std::string result = this->receive("VOLT1:LOW?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

std::string PG8133A::getFreq() {
     std::string result = this->receive("FREQ?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

std::string PG8133A::getFunc() {
     std::string result = this->receive("FUNC?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}

std::string PG8133A::getOutPen() {
     std::string result = this->receive("OUTP1?");
     logger(logDEBUG2) << __PRETTY_FUNCTION__ ;
     return result;
}
