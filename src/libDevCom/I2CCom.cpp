#include "I2CCom.h"

I2CCom::I2CCom(uint8_t deviceAddr)
  : m_deviceAddr(deviceAddr)
{ }

I2CCom::~I2CCom()
{ }

void I2CCom::setDeviceAddr(uint8_t deviceAddr)
{ m_deviceAddr=deviceAddr; }

uint8_t I2CCom::deviceAddr() const
{ return m_deviceAddr; }
