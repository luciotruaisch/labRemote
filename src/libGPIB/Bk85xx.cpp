#include "Bk85xx.h"
#include "Logger.h"

Bk85xx::Bk85xx(std::string dev) {
    m_address = 0;
    m_com = new SerialCom(dev);
    if (!m_com->is_open()) {
        log(logERROR) << __PRETTY_FUNCTION__ << " -> Could not open com port: " << dev;
    }
}

Bk85xx::~Bk85xx() {
    delete m_com;
}

void Bk85xx::send(union u_packet packet) {
    log(logDEBUG3) << __PRETTY_FUNCTION__ << " -> Writing packet! " << sizeof(packet);
    for (unsigned i=0; i<sizeof(packet); i++)
        log(logDEBUG4) << std::hex << "0x" << (unsigned)packet.bytes[i] << std::dec;
    m_com->write((char*)packet.bytes, sizeof(packet.bytes));
}

union u_packet Bk85xx::receive() {
    union u_packet answer;
    m_com->read((char*)answer.bytes, sizeof(answer.bytes));
    log(logDEBUG3) << __PRETTY_FUNCTION__ << " -> Read packet! " << sizeof(answer);
    for (unsigned i=0; i<sizeof(answer); i++)
        log(logDEBUG4) << std::hex << "0x" << (unsigned)answer.bytes[i] << std::dec;
    return answer;
}

void Bk85xx::checkStatus(struct s_packet packet) {
    if (packet.command != CMD_STATUS) {
        log(logERROR) << __PRETTY_FUNCTION__ << " -> Was asked to interpret status, but packet is not a status packet!";
    }
    
    std::string msg;
    switch (packet.data[0]) {
        case 0x90:
            msg = "Checksum incorrect";
            break;
        case 0xA0:
            msg = "Parameter incorrect";
            break;
        case 0xB0:
            msg = "Unregonized command";
            break;
        case 0xC0:
            msg = "Invalid command";
            break;
        case 0x80:
            msg = "Command was successful";
            break;
        default:
            msg = "Unknown status message";
            break;
    }
    
    if (packet.data[0] != 0x80) {
        log(logERROR) << __PRETTY_FUNCTION__ << " -> " << msg;
    } else {
        log(logDEBUG) << __PRETTY_FUNCTION__ << " -> " << msg;
    }
}

union u_packet Bk85xx::genPacket() {
    union u_packet tmp;
    // Just initialising with some defaults
    tmp.var.header = 0xAA;
    tmp.var.address = m_address;
    tmp.var.command = 0x0;
    for (unsigned int i=0; i<22; i++)
        tmp.var.data[i] = 0x0;
    tmp.var.checksum = 0x0; 
    return tmp;
}

void Bk85xx::setChecksum(union u_packet &packet) {
    unsigned sum = 0;
    for (unsigned int i=0; i<26; i++)
        sum += packet.bytes[i];
    sum = sum%256;
    packet.var.checksum = sum;
    log(logDEBUG2) << __PRETTY_FUNCTION__ << " -> " << sum;
}

struct s_packet Bk85xx::sendCommand(uint8_t cmd, uint8_t data[22]) {
    union u_packet tmp = this->genPacket();
    tmp.var.command = cmd;
    for (unsigned int i=0; i<22; i++)
        tmp.var.data[i] = data[i];
    this->setChecksum(tmp);
    this->send(tmp);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait)); 
    union u_packet response = this->receive();
    return response.var;
}

struct s_packet Bk85xx::sendCommand(uint8_t cmd) {
    union u_packet tmp = this->genPacket();
    tmp.var.command = cmd;
    this->setChecksum(tmp);
    this->send(tmp);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait)); 
    union u_packet response = this->receive();
    return response.var;
}

void Bk85xx::setRemote() {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    uint8_t data[22] = {0};
    data[0] = 1;
    this->checkStatus(this->sendCommand(CMD_REMOTE, data));
}

void Bk85xx::turnOn() {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    uint8_t data[22] = {0};
    data[0] = 1;
    this->checkStatus(this->sendCommand(CMD_ON_OFF, data));
}
    
void Bk85xx::turnOff() {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    uint8_t data[22] = {0};
    data[0] = 0;
    this->checkStatus(this->sendCommand(CMD_ON_OFF, data));
}

void Bk85xx::setModeCC() {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    uint8_t data[22] = {0};
    data[0] = 0;
    this->checkStatus(this->sendCommand(CMD_SET_MODE, data));
}

void Bk85xx::setModeCV() {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    uint8_t data[22] = {0};
    data[0] = 1;
    this->checkStatus(this->sendCommand(CMD_SET_MODE, data));
}

void Bk85xx::setModeCW() {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    uint8_t data[22] = {0};
    data[0] = 2;
    this->checkStatus(this->sendCommand(CMD_SET_MODE, data));
}

void Bk85xx::setModeCR() {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    uint8_t data[22] = {0};
    data[0] = 3;
    this->checkStatus(this->sendCommand(CMD_SET_MODE, data));
}

void Bk85xx::setCurrent(double current) {
    log(logDEBUG) << __PRETTY_FUNCTION__ << " -> " << current;
    uint32_t cur = current*10; // 0.1mA  = 0x1
    uint8_t data[22] = {0};
    data[0] = (cur & 0xFF);
    data[1] = ((cur >> 8) & 0xFF);
    data[2] = ((cur >> 16) & 0xFF);
    data[3] = ((cur >> 24) & 0xFF);
    this->checkStatus(this->sendCommand(CMD_SET_CC_CURRENT, data));
}
 
double Bk85xx::getCurrent() {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    struct s_packet ans = this->sendCommand(CMD_GET_CC_CURRENT);
    uint32_t cur = 0; // 0.1mA  = 0x1
    cur += (ans.data[0] & 0xFF);
    cur += ((ans.data[1] & 0xFF) << 8);
    cur += ((ans.data[2] & 0xFF) << 16);
    cur += ((ans.data[3] & 0xFF) << 24);
    return cur*0.1;
}

struct values Bk85xx::getValues() {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    struct s_packet ans = this->sendCommand(CMD_READ_VALUES);
    struct values val = {0, 0, 0};
    // 1mV  = 0x1
    val.vol += (ans.data[0] & 0xFF);
    val.vol += ((ans.data[1] & 0xFF) << 8);
    val.vol += ((ans.data[2] & 0xFF) << 16);
    val.vol += ((ans.data[3] & 0xFF) << 24);
    // 0.1mA  = 0x1
    val.cur += (ans.data[4] & 0xFF);
    val.cur += ((ans.data[5] & 0xFF) << 8);
    val.cur += ((ans.data[6] & 0xFF) << 16);
    val.cur += ((ans.data[7] & 0xFF) << 24);
    val.cur = val.cur*0.1;
    // 1mW  = 0x1
    val.pow += (ans.data[8] & 0xFF);
    val.pow += ((ans.data[9] & 0xFF) << 8);
    val.pow += ((ans.data[10] & 0xFF) << 16);
    val.pow += ((ans.data[11] & 0xFF) << 24);
    log(logDEBUG2) << __PRETTY_FUNCTION__ << " -> " << val.vol << " " << val.cur << " " << val.pow;
    return val;
}

void Bk85xx::setRemoteSense(bool enableRemoteSense) {
    log(logDEBUG) << __PRETTY_FUNCTION__;
    uint8_t data[22] = {0};
    data[0] = enableRemoteSense;
    this->checkStatus(this->sendCommand(CMD_SET_REMOTE_SENSE, data));
}
