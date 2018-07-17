#include "Keithley24XX.h"

#include <algorithm>

#include "Logger.h"

Keithley24XX::Keithley24XX(std::string dev, unsigned addr) {
  m_com = new SerialCom(dev, B115200);
  m_addr = addr;
  m_com->write("++auto 0\n\r");
}

Keithley24XX::~Keithley24XX() {
  delete m_com;
}

// TODO send/receive should be in prologix class
void Keithley24XX::send(std::string cmd) {
  m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
  log(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
  cmd += "\r\n";
  m_com->write(cmd);
  std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string Keithley24XX::receive(std::string cmd) {
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

void Keithley24XX::init() {
  // Check if the PS is connected
  std::string idn=this->receive("*idn?");
  idn.erase(std::find_if(idn.rbegin(), idn.rend(), [](int ch) {
        return !std::isspace(ch);
      }).base(), idn.end());

  if(idn.find("KEITHLEY INSTRUMENTS INC.,MODEL 2410,0692306,C12")==std::string::npos)
    throw "Unknown power supply: "+idn;
  return;
  // Prepare everything else
  this->send("OUTPUT OFF");
  this->send("*RST");
  this->send(":TRIGGER:COUNT 1");
  this->send(":FORMAT:ELEMENTS TIME,VOLT,CURR");
}

void Keithley24XX::turnOn() {
    this->send("OUTPUT ON");
}

void Keithley24XX::turnOff() {
    this->send("OUTPUT OFF");
}

void Keithley24XX::setSource(enum KeithleyMode mode, double range, double value) {
    switch (mode) {
        case KeithleyMode::VOLTAGE:
            this->send(":SOURCE:FUNC VOLT");
            this->send(":SOURCE:VOLT:RANGE " + std::to_string(range));
            this->send(":SOURCE:VOLT " + std::to_string(value));
            break;
        case KeithleyMode::CURRENT:
            this->send(":SOURCE:FUNC CURR");
            this->send(":SOURCE:CURR:RANGE " + std::to_string(range));
            this->send(":SOURCE:CURR " + std::to_string(value));
            break;
        default:
            log(logERROR) << __PRETTY_FUNCTION__ << " : Unknown mode!";
            break;
    }
}

void Keithley24XX::setSense(enum KeithleyMode mode, double range, double protection) {
    switch (mode) {
        case KeithleyMode::VOLTAGE:
            this->send(":SENSE:FUNC \"VOLT\"");
            this->send(":SENSE:VOLT:PROTECTION " + std::to_string(protection));
            this->send(":SENSE:VOLT:RANGE " + std::to_string(range));
            this->send(":FORMAT:ELEMENTS VOLT");
            break;
        case KeithleyMode::CURRENT:
            this->send(":SENSE:FUNC \"CURR\"");
            this->send(":SENSE:CURR:PROTECTION " + std::to_string(protection));
            this->send(":SENSE:CURR:RANGE " + std::to_string(range));
            this->send(":FORMAT:ELEMENTS CURR");
            break;
        default:
            log(logERROR) << __PRETTY_FUNCTION__ << " : Unknown mode!";
            break;
    }
}

std::string Keithley24XX::sense(enum KeithleyMode mode) {
  switch (mode) {
  case KeithleyMode::VOLTAGE:
    this->send(":SENSE:FUNC \"VOLT\"");
    this->send(":FORMAT:ELEMENTS VOLT");
    break;
  case KeithleyMode::CURRENT:
    this->send(":SENSE:FUNC \"CURR\"");
    this->send(":FORMAT:ELEMENTS CURR");
    break;
  default:
    log(logERROR) << __PRETTY_FUNCTION__ << " : Unknown mode!";
    break;
  }

  return this->receive(":READ?").substr(0, 13);
}

