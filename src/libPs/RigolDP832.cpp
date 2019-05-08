#include "RigolDP832.h"
#include "Logger.h"
#include <stdlib.h>

RigolDP832::RigolDP832(std::string dev) {
    m_com = new SerialCom(dev);
    //m_com->write("++auto 0\n\r");
    //m_com->write("USB0::0x1AB1::0x0E11::DP8C192903403::INSTR \n\r");
}

RigolDP832::~RigolDP832() {
    delete m_com;
}

void RigolDP832::send(std::string cmd) {
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    //cmd += "\n\r";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string RigolDP832::receive(std::string cmd) {
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    //cmd += "\n\r";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
    std::string buf;
    m_com->read(buf);
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Received: " << buf;
    return buf;
}

void RigolDP832::init() {
    //this->send(":MONI:STOP OUTOFF,OFF");
    this->send("*RST");
    //this->send(":SYSTem:BEEPer:IMMediate"); //beep
    //this->send(":SYSTEM:REMOTE");
    std::string result=this->receive("*IDN?");
    //std::cout<<result<<std::endl;
    //this->send(":TRIGGER:COUNT 1");
    //this->send(":FORMAT:ELEMENTS TIME,VOLT,CURR");
}

void RigolDP832::turnOn(unsigned channel) {
    this->send(":OUTP CH" + std::to_string(channel) + ",ON");
}

void RigolDP832::turnOff(unsigned channel) {
    this->send(":OUTP CH" + std::to_string(channel) + ",OFF");
}

void RigolDP832::setVoltageCurrent(unsigned channel, float volt, float cur) {
    this->send(":APPL CH" + std::to_string(channel) + ", " + std::to_string(volt) + ", " + std::to_string(cur));
}

std::string RigolDP832::getVoltageStr(unsigned channel) {
    return this->receive(":MEAS? CH" + std::to_string(channel));
}

std::string RigolDP832::getCurrentStr(unsigned channel) {
    return this->receive(":MEAS:CURR? CH" + std::to_string(channel));
}

float RigolDP832::getVoltage(unsigned channel) {
  std::string v = getVoltageStr(channel);
  return atof(v.c_str());
}

float RigolDP832::getCurrent(unsigned channel) {
  std::string i = getCurrentStr(channel);
  return atof(i.c_str());  
}

