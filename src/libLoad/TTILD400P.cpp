#include "TTILD400P.h"

#include <algorithm>

#include "Logger.h"


TTILD400P::TTILD400P(std::string dev, unsigned addr) {
  m_com = new SerialCom(dev, B115200);
  m_addr = addr;
  m_com->write("++auto 0\n\r");
  m_channel = 0;
}

TTILD400P::~TTILD400P() {
    delete m_com;
}

void TTILD400P::send(std::string cmd){
  m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
  logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
  cmd += "\r\n";
  m_com->write(cmd);
  std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string TTILD400P::receive(std::string cmd){
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

void TTILD400P::init() {
  // Check if the Load is connected
  std::string idn=this->receive("*idn?");
  idn.erase(std::find_if(idn.rbegin(), idn.rend(), [](int ch) {
	return !std::isspace(ch);
      }).base(), idn.end());
  
  //TBD what the idn is 
  if(idn.find("THURLBY THANDAR, LD400P, 480850, 1.04-1.05") == std::string::npos)
    throw "Unknown Load: "+idn;
  
  // Prepare everything else
 
  this->send("*RST");
  std::this_thread::sleep_for(std::chrono::seconds(1));
}


void TTILD400P::setVoltage(double volt) {
  this->send("A "+std::to_string(volt));
  this->send("LVLSEL A");
}

std::string TTILD400P::getVoltage() {
  std::string result=this->receive("V?");
  return result.substr(0, result.length()-2);
}

void TTILD400P::setVoltageLim(double vol) {
  this->send("VLIM "+std::to_string(vol));
}

void TTILD400P::setCurrent(double cur) {
  this->send("A "+std::to_string(cur));
  this->send("LVLSEL A");
}

std::string TTILD400P::getCurrent() {
  std::string result=this->receive("I?");
  return result.substr(0, result.length()-2);
}

void TTILD400P::setCurrentLim(double cur) {
  this->send("ILIM "+std::to_string(cur));
}


void TTILD400P::turnOn(){
  this->send("INP 1");
}

void TTILD400P::turnOff(){
  this->send("INP 0");
}

void TTILD400P::setModeCC(){
this->send("MODE C");
}

void TTILD400P::setModeCV(){
this->send("MODE V");
}

void TTILD400P::setModeCW(){
this->send("MODE P");
}

void TTILD400P::setModeCR(){
this->send("MODE R");
}

std::string TTILD400P::getMode(){
  std::string result=this->receive("MODE?");
  return result;
}
 
