#ifndef BK85XX_H
#define BK85XX_H

// ####################
// BK85xx Programmable DC load
// Author: Timon Heim
// Date: Feb 2017
// Notes: https://bkpmedia.s3.amazonaws.com/downloads/manuals/en-us/85xx_manual.pdf
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "SerialCom.h"

#define CMD_STATUS 0x12
#define CMD_REMOTE 0x20
#define CMD_ON_OFF 0x21

#define CMD_SET_MAX_VOLTAGE 0x22
#define CMD_GET_MAX_VOLTAGE 0x23
#define CMD_SET_MAX_CURRENT 0x24
#define CMD_GET_MAX_CURRENT 0x25
#define CMD_SET_MAX_POWER 0x26
#define CMD_GET_MAX_POWER 0x27

#define CMD_SET_MODE 0x28
#define CMD_GET_MODE 0x29

#define CMD_SET_CC_CURRENT 0x2A
#define CMD_GET_CC_CURRENT 0x2B
#define CMD_SET_CV_VOLTAGE 0x2C
#define CMD_GET_CV_VOLTAGE 0x2D
#define CMD_SET_CW_POWER 0x2E
#define CMD_GET_CW_POWER 0x2F
#define CMD_SET_CR_RESISTANCE 0x30
#define CMD_GET_CR_RESISTANCE 0x31

// Lots of other cmds
#define CMD_SET_REMOTE_SENSE 0x56
#define CMD_READ_VALUES 0x5F

// more calib cmds

#define CMD_GET_INFO 0x6A

struct s_packet {
    uint8_t header;
    uint8_t address;
    uint8_t command;
    uint8_t data[22];
    uint8_t checksum;
};

union u_packet {
    uint8_t bytes[26];
    struct s_packet var;
};

struct values {
    double vol;
    double cur;
    double pow;
};

class Bk85xx {
    public:
        Bk85xx(std::string dev);
        ~Bk85xx();

        void setRemote();

        void turnOn();
        void turnOff();

        void setModeCC();
        void setModeCV();
        void setModeCW();
        void setModeCR();

        void setRemoteSense(bool enableRemoteSense=true);

        void setCurrent(double current); // mA
        double getCurrent();
        struct values getValues();

    private:
        SerialCom *m_com;
        uint8_t m_address;

        void send(union u_packet packet);
        union u_packet receive();
        void checkStatus(struct s_packet packet);
        
        union u_packet genPacket();
        struct s_packet sendCommand(uint8_t cmd, uint8_t data[22]);
        struct s_packet sendCommand(uint8_t cmd);
        void setChecksum(union u_packet &packet);

        const std::chrono::milliseconds m_wait{200};
};

#endif
