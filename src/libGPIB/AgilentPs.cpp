#include "AgilentPs.h"

#include <algorithm>

#include "Logger.h"

loglevel_e loglevel;


AgilentPs::AgilentPs(std::string dev, unsigned addr) {
  m_com = new SerialCom(dev, B115200);
  m_addr = addr;
  m_com->write("++auto 0\n\r");
}

AgilentPs::~AgilentPs() {
  delete m_com;
}

void AgilentPs::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string AgilentPs::receive(std::string cmd) 
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

void AgilentPs::init() {
  // Check if the PS is connected
  std::string idn=this->receive("*idn?");
  idn.erase(std::find_if(idn.rbegin(), idn.rend(), [](int ch) {
        return !std::isspace(ch);
      }).base(), idn.end());

  if(idn!="HEWLETT-PACKARD,E3633A,0,2.4-6.1-2.1")
    throw "Unknown power supply: "+idn;

  // Prepare everything else
  this->send("OUTPUT OFF");
  this->send("*RST");
  this->send("TRIGGER:SOURCE IMM");
  this->send("VOLT:PROT MAX");
}


void AgilentPs::setCh(unsigned ch) {
    this->send("INST:SEL OUT" + std::to_string(ch));
}

void AgilentPs::setRange(unsigned range) {
    this->send("VOLTAGE:RANGE P" + std::to_string(range) + "V");
}

void AgilentPs::setVoltage(double volt) {
    this->send("VOLTAGE " + std::to_string(volt));
}

std::string AgilentPs::getVoltage() {
    std::string result=this->receive("MEAS:VOLT?");
    return result.substr(0, result.length()-1);
}

void AgilentPs::setCurrent(double cur) {
    this->send("CURRENT " + std::to_string(cur));
}

std::string AgilentPs::getCurrent() {
    return this->receive("MEAS:CURR?").substr(0, 15);
}

void AgilentPs::turnOn() {
    this->send("OUTPUT ON");
}


void AgilentPs::turnOff() {
    this->send("OUTPUT OFF");
}


