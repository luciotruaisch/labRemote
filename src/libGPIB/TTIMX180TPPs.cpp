#include "TTIMX180TPPs.h"

#include <algorithm>

#include "Logger.h"

//loglevel_e loglevel;


TTIMX180TPPs::TTIMX180TPPs(std::string dev, unsigned addr) {
  m_com = new SerialCom(dev, B115200);
  m_addr = addr;
  m_com->write("++auto 0\n\r");
}

TTIMX180TPPs::~TTIMX180TPPs() {
  delete m_com;
}

void TTIMX180TPPs::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    log(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string TTIMX180TPPs::receive(std::string cmd) 
{
  m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
  log(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
  cmd += "\r\n";
  m_com->write(cmd);
  m_com->write("++read eoi\n\r");
  std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
  std::string buf;
  m_com->read(buf);
  log(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Received: " << buf;
  return buf;
}

void TTIMX180TPPs::init() {
  // Check if the PS is connected
  std::string idn=this->receive("*idn?");
  idn.erase(std::find_if(idn.rbegin(), idn.rend(), [](int ch) {
        return !std::isspace(ch);
      }).base(), idn.end());

  if(idn!="THURLY THORNTON,TSX1820P,0,1.00-1.00")
    throw "Unknown power supply: "+idn;

  // Prepare everything else
  this->send("OPALL OFF");
  this->send("*RST");
  //  this->send("TRIGGER:SOURCE IMM");
  //this->send("VOLT:PROT MAX");
}


void TTIMX180TPPs::setCh(unsigned ch) {
    this->send("INST:SEL OUT" + std::to_string(ch));
}

void TTIMX180TPPs::setRange(unsigned number,unsigned range) {
    this->send("VRANGE" + std::to_string(number)+std::to_string(range) + "V");
}

void TTIMX180TPPs::setVoltage(unsigned number,double volt) {
    this->send("V " + std::to_string(number)+std::to_string(volt));
}

std::string TTIMX180TPPs::getVoltage(unsigned number) {
    std::string result=this->receive("V"+std::to_string(number)+"?");
    return result.substr(0, result.length()-2);
}

void TTIMX180TPPs::setCurrent(unsigned number,double cur) {
    this->send("I" + std::to_string(number)+std::to_string(cur));
}

std::string TTIMX180TPPs::getCurrent(unsigned number) {
    return this->receive("I"+std::to_string(number)+"?").substr(0, 15);
}

void TTIMX180TPPs::turnOn(unsigned number) {
    this->send("OP" + std::to_string(number) + "ON");
}


void TTIMX180TPPs::turnOff(unsigned number) {
  this->send("OP" + std::to_string(number) + "OFF");
}


