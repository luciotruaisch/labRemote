#include "Fluke8842.h"
#include "Logger.h"

Fluke8842::Fluke8842(std::string dev, unsigned addr) {
    m_com = new SerialCom(dev, B115200);
    m_addr = addr;
    m_com->write("++auto 0\n\r");
}

Fluke8842::~Fluke8842() {
    delete m_com;
}

// TODO send/receive should be in prologix class
void Fluke8842::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    log(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string Fluke8842::receive(std::string cmd) {
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

void Fluke8842::init() {
    this->send("*");
}

void Fluke8842::setSense(enum FlukeMode mode) {
    switch (mode) {
        case FlukeMode::VOLTAGEDC:
            this->send("F1 R0 S1 T2");
            break;
        case FlukeMode::VOLTAGEAC:
            this->send("F2 R0 S1 T2");
            break;
        case FlukeMode::CURRENTDC:
            this->send("F5 R0 S1 T2");
            break;
        case FlukeMode::CURRENTAC:
            this->send("F6 R0 S1 T2");
            break;
        default:
            log(logERROR) << __PRETTY_FUNCTION__ << " : Unknown mode!";
            break;
    }
}

std::string Fluke8842::sense() {
    return this->receive("?").substr(0, 11);
}


