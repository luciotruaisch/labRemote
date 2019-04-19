#include "Keithley2000.h"
#include "Logger.h"

Keithley2000::Keithley2000(std::string dev, unsigned addr) {
    m_com = new SerialCom(dev, B115200);
    m_addr = addr;
    m_com->write("++auto 0\n\r");
}

Keithley2000::~Keithley2000() {
    delete m_com;
}

// TODO send/receive should be in prologix class
void Keithley2000::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string Keithley2000::receive(std::string cmd) {
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

void Keithley2000::init() {
    this->send("*RST");
    this->send(":FORMAT:ELEMENTS READ,CHAN,UNIT");
    this->send(":SYST:BEEP:STAT OFF");//disable beep
}

void Keithley2000::setSense(enum KeithleyMode mode) {
    switch (mode) {
        case KeithleyMode::VOLTAGE:
            this->send(":SENSE:FUNC \"VOLT\"");
            break;
        case KeithleyMode::CURRENT:
            this->send(":SENSE:FUNC \"CURR\"");
            break;
        default:
            logger(logERROR) << __PRETTY_FUNCTION__ << " : Unknown mode!";
            break;
    }
}

std::string Keithley2000::sense() {
    return this->receive(":READ?").substr(0, 13);
}

//SCAN scanner board -- Switching Matrix Functions

std::string Keithley2000::readChannel(std::string channel){
    this->send(":ROUTE:OPEN:ALL");
    this->send(":SENSE:FUNC \"VOLT\"");
    this->send(":ROUTE:CLOSE (@" + channel + ")");
    return this->receive(":READ?").substr(0,13);
}



