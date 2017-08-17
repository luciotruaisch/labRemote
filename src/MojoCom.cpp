#include "MojoCom.h"

MojoCom::MojoCom(SerialCom *com) {
    m_com = com;
}

int MojoCom::writeReg(unsigned reg, unsigned val) {
    char outbuf[7], inbuf[4];

    // Prepare buffer
    outbuf[0] = 0x80 | ((reg >> 8) & 0xFF);
    outbuf[1] = reg & 0xFF;
    outbuf[2] = (val >> 24 ) & 0xFF;
    outbuf[3] = (val >> 16 ) & 0xFF;
    outbuf[4] = (val >> 8 ) & 0xFF;
    outbuf[5] = val & 0xFF;
    outbuf[6] = '\n';

    // Write buffer
    if (m_com->write(outbuf, 7) < 7) return -1;
    // Read status
    if (m_com->read(inbuf, 2) < 2) return -1;
    // Check status
    if ((inbuf[0] & 0xF) > 0) return -1;
    if (inbuf[1] != '\n') return -1;

    return 0;
}

int MojoCom::readReg(unsigned reg, unsigned &val) {
    char outbuf[3], inbuf[8];

    // Prepare output
    outbuf[0] = 0x00 | ((reg >> 8) & 0xFF);
	outbuf[1] = reg & 0xFF;
	outbuf[2] = '\n';

    // Write output
    if (m_com->write(outbuf, 3) < 3) return -1;
    // Read status
    if (m_com->read(inbuf, 2) < 2) return -1;
    // Check status
    if (!(inbuf[0] & 0x8)) return -1;
    // Read data
    if (m_com->read(&inbuf[2], 4) < 4) return -1;
    // Check status
    if (inbuf[5] != '\n') return -1;
    // Trasnfer data
    for(int i = 1; i < 5; i++) val = (val<<8) + (unsigned char)inbuf[i];

    return 0;
}

int MojoCom::enableI2C() {
    // Do some magic
    if (this->writeReg(0x100, 0x00)) return -1;
    if (this->writeReg(0x101, 0x01)) return -1;
    if (this->writeReg(0x102, 0x80)) return -1;
    return 0;
}

int MojoCom::readI2C(unsigned id, unsigned addr, unsigned *data, unsigned bytes) {
    unsigned i(0), reg(0);

	// Write Slave Address(RW = W) + Start Condition
	if(writeReg(0x103, id<<1)) return -1;
	if(writeReg(0x104,0x90)) return -1;
	do{
		usleep(1000);
		if(readReg(0x104, reg)) return -1;
	}while(reg & 0x02);
	if(reg != 0x41)
		return -1;

	// Write Register Address
	if(writeReg(0x103, addr)) return -1;
	if(writeReg(0x104,0x10)) return -1;
	do{
		usleep(1000);
		if(readReg(0x104, reg)) return -1;
	}while(reg & 0x02);
	if(reg != 0x41)
		return -1;

	// Write Slave Address(RW = R) + Repeated Start Condition
	if(writeReg(0x103, id*2+1)) return -1;
	if(writeReg(0x104,0x90)) return -1;
	do{
		usleep(1000);
		if(readReg(0x104, reg)) return -1;
	}while(reg & 0x02);
	if(reg != 0x41)
		return -1;

	// Read Bytes
	for(i = 0; i < (bytes - 1); i++){;
		if(writeReg(0x104,0x20)) return -1;
		do{
			usleep(1000);
			if(readReg(0x104, reg)) return -1;
		}while(reg & 0x02);
		if(reg != 0x41)
			return -1;
		if(readReg(0x103, data[i])) return -1;
	}

	// Read last byte + Stop Condition & NACK
	if(writeReg(0x104,0x68)) return -1;
	do{
		usleep(1000);
		if(readReg(0x104, reg)) return -1;
	}while(reg & 0x02);
	if(reg != 0x81)
		return -1;
	if(readReg(0x103, data[i])) return -1;

	return 0;
}

int MojoCom::writeI2C(unsigned id, unsigned addr, unsigned *data, unsigned bytes) {
	unsigned i(0), reg(0);

    // Write id + Start Condition
	if(writeReg(0x103, id<<1)) return -1;
	if(writeReg(0x104,0x90)) return -1;
	do{
		usleep(1000);
		if(readReg(0x104, reg)) return -1;
	}while(reg & 0x02);
	if(reg != 0x41)
		return -1;

    // Write reg addr
    if(writeReg(0x103, addr)) return -1;
    if(writeReg(0x104,0x10)) return -1;
    do{
        usleep(1000);
        if(readReg(0x104, reg)) return -1;
    }while(reg & 0x02);
    if(reg != 0x41)
        return -1;

	// Write Bytes
	for(i = 0; i < (bytes-1); i++){
		if(writeReg(0x103, data[i])) return -1;
		if(writeReg(0x104,0x10)) return -1;
		do{
			usleep(1000);
			if(readReg(0x104, reg)) return -1;
		}while(reg & 0x02);
		if(reg != 0x41)
			return -1;
	}
	
	// Write Byte + Stop Condition
	if(writeReg(0x103, data[i])) return -1;
	if(writeReg(0x104,0x50)) return -1;
	do{
		usleep(1000);
		if(readReg(0x104, reg)) return -1;
	}while(reg & 0x02);
	if(reg != 0x01)
		return -1;

	return 0;
}
