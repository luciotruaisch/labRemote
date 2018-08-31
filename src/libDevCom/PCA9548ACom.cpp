#include "PCA9548ACom.h"

PCA9548ACom::PCA9548ACom(uint8_t deviceAddr, uint8_t channel, std::shared_ptr<I2CCom> com)
  : I2CCom(deviceAddr),
    m_com(com), m_muxAddr(com->deviceAddr()), m_channel(channel)
{ }

PCA9548ACom::~PCA9548ACom()
{ }

void PCA9548ACom::write_reg32(uint32_t address, uint32_t data)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  m_com->write_reg32(address, data);
  m_com->setDeviceAddr(m_muxAddr);
}

void PCA9548ACom::write_reg16(uint32_t address, uint16_t data)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  m_com->write_reg16(address, data);
  m_com->setDeviceAddr(m_muxAddr);
}

void PCA9548ACom::write_reg8 (uint32_t address, uint8_t  data)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  m_com->write_reg8 (address, data);
  m_com->setDeviceAddr(m_muxAddr);
}

void PCA9548ACom::write_reg32(uint32_t data)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  m_com->write_reg32(data);
  m_com->setDeviceAddr(m_muxAddr);
}

void PCA9548ACom::write_reg16(uint16_t data)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  m_com->write_reg16(data);
  m_com->setDeviceAddr(m_muxAddr);
}

void PCA9548ACom::write_reg8 (uint8_t  data)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  m_com->write_reg8 (data);
  m_com->setDeviceAddr(m_muxAddr);
}

void PCA9548ACom::write_block(uint32_t address, const std::vector<uint8_t>& data)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  m_com->write_block(address, data);
  m_com->setDeviceAddr(m_muxAddr);
}

void PCA9548ACom::write_block(const std::vector<uint8_t>& data)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  m_com->write_block(data);
  m_com->setDeviceAddr(m_muxAddr);
}

uint32_t PCA9548ACom::read_reg32(uint32_t address)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  uint32_t data=m_com->read_reg32(address);
  m_com->setDeviceAddr(m_muxAddr);

  return data;
}

uint16_t PCA9548ACom::read_reg16(uint32_t address)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  uint16_t data=m_com->read_reg16(address);
  m_com->setDeviceAddr(m_muxAddr);

  return data;
}

uint8_t  PCA9548ACom::read_reg8 (uint32_t address)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  uint8_t data=m_com->read_reg8 (address);
  m_com->setDeviceAddr(m_muxAddr);

  return data;
}

uint32_t PCA9548ACom::read_reg32()
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  uint32_t data=m_com->read_reg32();
  m_com->setDeviceAddr(m_muxAddr);

  return data;
}

uint16_t PCA9548ACom::read_reg16()
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  uint16_t data=m_com->read_reg16();
  m_com->setDeviceAddr(m_muxAddr);

  return data;
}

uint8_t  PCA9548ACom::read_reg8 ()
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  uint8_t data=m_com->read_reg8 ();
  m_com->setDeviceAddr(m_muxAddr);

  return data;
}

void PCA9548ACom::read_block(uint32_t address, std::vector<uint8_t>& data)
{
  m_com->write_reg8(1<<m_channel);

  m_com->setDeviceAddr(deviceAddr());
  m_com->read_block(address, data);
  m_com->setDeviceAddr(m_muxAddr);
}

void PCA9548ACom::read_block(std::vector<uint8_t>& data)
{
  m_com->write_reg8(1<<m_channel);

  uint8_t m_muxAddr=m_com->deviceAddr();
  m_com->setDeviceAddr(deviceAddr());
  m_com->read_block(data);
  m_com->setDeviceAddr(m_muxAddr);
 
}
