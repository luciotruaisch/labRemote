#include "TTIMX180TPPs.h"

#include <algorithm>

#include "Logger.h"

//loglevel_e loglevel;


TTIMX180TPPs::TTIMX180TPPs(std::string dev, unsigned addr) {
    m_com = new SerialCom(dev, B115200);
    m_addr = addr;
    m_com->write("++auto 0\n\r");
    m_channel = 0;
}

TTIMX180TPPs::~TTIMX180TPPs() {
    delete m_com;
}

void TTIMX180TPPs::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string TTIMX180TPPs::receive(std::string cmd) 
{
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

void TTIMX180TPPs::init() {
    // Check if the PS is connected
    std::string idn=this->receive("*idn?");
    idn.erase(std::find_if(idn.rbegin(), idn.rend(), [](int ch) {
                return !std::isspace(ch);
                }).base(), idn.end());

    if(idn.find("THURLBY THANDAR, MX180TP") == std::string::npos)
        throw "Unknown power supply: "+idn;

    // Prepare everything else
    this->send("OPALL 0");
    this->send("*RST");
}


void TTIMX180TPPs::setCh(unsigned ch) {
    m_channel = ch;
}

void TTIMX180TPPs::setRange(unsigned range) {

}

void TTIMX180TPPs::setVoltage(double volt) {
    this->send("V" + std::to_string(m_channel)+" "+std::to_string(volt));
}

std::string TTIMX180TPPs::getVoltage() {
    std::string result=this->receive("V"+std::to_string(m_channel)+"O?");
    return result.substr(0, result.length()-2);
}

void TTIMX180TPPs::setCurrent(double cur) {
    this->send("I" + std::to_string(m_channel)+" "+std::to_string(cur));
}

std::string TTIMX180TPPs::getCurrent() {
    std::string result=this->receive("I"+std::to_string(m_channel)+"O?");
    return result.substr(0, result.length()-2);
}

void TTIMX180TPPs::turnOn() {
    this->send("OP" + std::to_string(m_channel) + " 1");
}


void TTIMX180TPPs::turnOff() {
    this->send("OP" + std::to_string(m_channel) + " 0");
}


