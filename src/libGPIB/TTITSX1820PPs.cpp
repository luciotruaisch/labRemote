#include "TTITSX1820PPs.h"

#include <algorithm>

#include "Logger.h"



TTITSX1820PPs::TTITSX1820PPs(std::string dev, unsigned addr) {
  m_com = new SerialCom(dev, B115200);
  m_addr = addr;
  m_com->write("++auto 0\n\r");
}

TTITSX1820PPs::~TTITSX1820PPs() {
  delete m_com;
}

void TTITSX1820PPs::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string TTITSX1820PPs::receive(std::string cmd) 
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

void TTITSX1820PPs::init() {
  // Check if the PS is connected
  std::string idn=this->receive("*idn?");
  idn.erase(std::find_if(idn.rbegin(), idn.rend(), [](int ch) {
        return !std::isspace(ch);
      }).base(), idn.end());

  if(idn!="THURLBY-THANDAR,TSX1820P,0,1.20")
    throw "Unknown power supply: "+idn;

  // Prepare everything else
  this->send("OP 0");
  this->send("*RST");
  //  this->send("TRIGGER:SOURCE IMM");
  //this->send("VOLT:PROT MAX");
}


void TTITSX1820PPs::setCh(unsigned ch) {
}

void TTITSX1820PPs::setRange(unsigned range) {
}

void TTITSX1820PPs::setVoltage(double volt) {
    this->send("V " + std::to_string(volt));
}

std::string TTITSX1820PPs::getVoltage() {
    std::string result=this->receive("VO?");
    return result.substr(0, result.length()-2);
}

void TTITSX1820PPs::setCurrent(double cur) {
    this->send("I " + std::to_string(cur));
}

std::string TTITSX1820PPs::getCurrent() {
    std::string result=this->receive("IO?");
    return result.substr(0, result.length()-2);
}

void TTITSX1820PPs::turnOn() {
    this->send("OP 1");
}


void TTITSX1820PPs::turnOff() {
    this->send("OP 0");
}


