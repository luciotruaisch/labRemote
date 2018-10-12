#include "SorensenPs.h"
#include "Logger.h"

SorensenPs::SorensenPs(std::string dev, unsigned addr)
{
  m_com = new SerialCom(dev, B115200);
  m_addr = addr;
  m_com->write("++auto 0\n\r");
}

SorensenPs::~SorensenPs()
{
  delete m_com;
}

void SorensenPs::send(std::string cmd)
{
  m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
  logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
  cmd += "\r\n";
  m_com->write(cmd);
  std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string SorensenPs::receive(std::string cmd)
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

void SorensenPs::init()
{
  this->send("OP1 0");
  this->send("*RST");
}


void SorensenPs::setCh(unsigned ch)
{
  m_ch=ch;
}

void SorensenPs::setVoltage(double volt)
{
  this->send("V"+std::to_string(m_ch)+" " + std::to_string(volt));
}

std::string SorensenPs::getVoltage()
{
  return this->receive("V"+std::to_string(m_ch)+"O?").substr(0, 5);
}

void SorensenPs::setCurrent(double cur)
{
  this->send("I"+std::to_string(m_ch)+" " + std::to_string(cur));
}

double SorensenPs::getCurrent()
{
  return std::stof(this->receive("I"+std::to_string(m_ch)+"O?").substr(0, 5));
}

void SorensenPs::turnOn()
{
  this->send("OP"+std::to_string(m_ch)+" 1");
}


void SorensenPs::turnOff()
{
  this->send("OP"+std::to_string(m_ch)+" 0");
}


