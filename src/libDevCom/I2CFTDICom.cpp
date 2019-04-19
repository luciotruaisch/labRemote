#include "I2CFTDICom.h"

extern "C"{
#include <mpsse.h>
}

#include <string.h>
#include <unistd.h>

#include "ComIOException.h"

I2CFTDICom::I2CFTDICom(uint8_t deviceAddr)
  : I2CCom(deviceAddr)
{
  if((m_i2cdev=MPSSE(I2C, ONE_HUNDRED_KHZ, MSB)) == NULL || !m_i2cdev->open) 
    throw ComIOException("Unable to create MPSSE device.");

  Tristate(m_i2cdev);
}

I2CFTDICom::~I2CFTDICom()
{ }

void I2CFTDICom::write_reg32(uint32_t address, uint32_t data)
{
  std::vector<uint8_t> datavec = {static_cast<uint8_t>((data>>24)&0xFF),
				  static_cast<uint8_t>((data>>16)&0xFF),
				  static_cast<uint8_t>((data>> 8)&0xFF),
				  static_cast<uint8_t>((data>> 0)&0xFF)};

  write_block(address, datavec);
}

void I2CFTDICom::write_reg16(uint32_t address, uint16_t data)
{
  std::vector<uint8_t> datavec = {static_cast<uint8_t>((data>> 8)&0xFF),
				  static_cast<uint8_t>((data>> 0)&0xFF)};

  write_block(address, datavec);
}

void I2CFTDICom::write_reg8(uint32_t address, uint8_t data)
{
  std::vector<uint8_t> datavec = {static_cast<uint8_t>((data>> 0)&0xFF)};

  write_block(address, datavec);
}

void I2CFTDICom::write_reg32(uint32_t data)
{
  std::vector<uint8_t> datavec = {static_cast<uint8_t>((data>>24)&0xFF),
				  static_cast<uint8_t>((data>>16)&0xFF),
				  static_cast<uint8_t>((data>> 8)&0xFF),
				  static_cast<uint8_t>((data>> 0)&0xFF)};

  write_block(datavec);
}

void I2CFTDICom::write_reg16(uint16_t data)
{
  std::vector<uint8_t> datavec = {static_cast<uint8_t>((data>> 8)&0xFF),
				  static_cast<uint8_t>((data>> 0)&0xFF)};

  write_block(datavec);
}

void I2CFTDICom::write_reg8(uint8_t data)
{
  std::vector<uint8_t> datavec = {static_cast<uint8_t>((data>> 0)&0xFF)};

  write_block(datavec);
}

void I2CFTDICom::write_block(uint32_t address, const std::vector<uint8_t>& data)
{
  if(Start(m_i2cdev)!=0)
    throw ComIOException("I2CFTDICom cannot sent START.");

  char buf[1]={static_cast<int8_t>(deviceAddr()<<1)};
  if(Write(m_i2cdev, buf, 1)!=0)
    throw ComIOException("I2CFTDICom cannot WRITE device address.");

  buf[0]=static_cast<int8_t>(address&0xFF);
  if(Write(m_i2cdev, buf, 1)!=0)
    throw ComIOException("I2CFTDICom cannot WRITE register address.");

  if(Write(m_i2cdev, const_cast<char*>(reinterpret_cast<const char*>(&data[0])), data.size())!=0)
    throw ComIOException("I2CFTDICom cannot WRITE data.");

  if(Stop(m_i2cdev)!=0) 
    throw ComIOException("I2CFTDICom cannot send STOP.");
}

void I2CFTDICom::write_block(const std::vector<uint8_t>& data)
{
  if(Start(m_i2cdev)!=0)
    throw ComIOException("I2CFTDICom cannot sent START.");

  char buf[1]={static_cast<int8_t>(deviceAddr()<<1)};
  if(Write(m_i2cdev, buf, 1)!=0)
    throw ComIOException("I2CFTDICom cannot WRITE device address.");

  if(Write(m_i2cdev, const_cast<char*>(reinterpret_cast<const char*>(&data[0])), data.size())!=0)
    throw ComIOException("I2CFTDICom cannot WRITE data.");

  if(Stop(m_i2cdev)!=0) 
    throw ComIOException("I2CFTDICom cannot send STOP.");
}

uint32_t I2CFTDICom::read_reg32(uint32_t address)
{
  std::vector<uint8_t> data(4);
  read_block(address, data);
  return (data[0]<<24)|(data[1]<<16)|(data[2]<<8)|(data[3]<<0);
}

uint16_t I2CFTDICom::read_reg16(uint32_t address)
{
  std::vector<uint8_t> data(2);
  read_block(address, data);
  return (data[0]<<8)|(data[1]<<0);
}

uint8_t I2CFTDICom::read_reg8(uint32_t address)
{
  std::vector<uint8_t> data(1);
  read_block(address, data);
  return (data[0]<<0);
}

uint32_t I2CFTDICom::read_reg32()
{
  std::vector<uint8_t> data(4);
  read_block(data);
  return (data[0]<<24)|(data[1]<<16)|(data[2]<<8)|(data[3]<<0);
}

uint16_t I2CFTDICom::read_reg16()
{
  std::vector<uint8_t> data(2);
  read_block(data);
  return (data[0]<<8)|(data[1]<<0);
}

uint8_t I2CFTDICom::read_reg8()
{
  std::vector<uint8_t> data(1);
  read_block(data);
  return (data[0]<<0);
}

void I2CFTDICom::read_block(uint32_t address, std::vector<uint8_t>& data)
{
  char buf[1] = {0};
  char *outbuf;

  if(Start(m_i2cdev)!=0)
    throw ComIOException("I2CFTDICom cannot sent START.");

  // Write the address
  buf[0]=static_cast<int8_t>(deviceAddr()<<1);
  if(Write(m_i2cdev, buf, 1)!=0)
    throw ComIOException("I2CFTDICom cannot WRITE device address.");

  buf[0]=static_cast<int8_t>(address&0xFF);
  if(Write(m_i2cdev, buf, 1)!=0) 
    throw ComIOException("I2CFTDICom cannot WRITE register address.");

  // Read the data
  if(GetAck(m_i2cdev)==ACK)
    {
      if(Start(m_i2cdev)!=0) 
	throw ComIOException("I2CFTDICom cannot sent START.");
      
      buf[0]=static_cast<int8_t>((deviceAddr()<<1)|1);
      if(Write(m_i2cdev, buf, 1)!=0)
	throw ComIOException("I2CFTDICom cannot WRITE device address.");

      SendAcks(m_i2cdev);
      outbuf=Read(m_i2cdev, data.size());
      for(uint32_t i=0;i<data.size();i++)
	data[i]=outbuf[i];

      SendNacks(m_i2cdev);
      Read(m_i2cdev, 1); // dummy read to send the NACK
    }
  else
    throw ComIOException("I2CFTDICom did not see an ACK.");

  if(Stop(m_i2cdev)!=0) 
    throw ComIOException("I2CFTDICom cannot send STOP.");
}

void I2CFTDICom::read_block(std::vector<uint8_t>& data)
{
  char buf[1] = {0};
  char *outbuf;

  if(Start(m_i2cdev)!=0)
    throw ComIOException("I2CFTDICom cannot sent START.");

  // Read the data
  if(GetAck(m_i2cdev)==ACK)
    {
      if(Start(m_i2cdev)!=0) 
	throw ComIOException("I2CFTDICom cannot sent START.");
      
      buf[0]=static_cast<int8_t>((deviceAddr()<<1)|1);
      if(Write(m_i2cdev, buf, 1)!=0)
	throw ComIOException("I2CFTDICom cannot WRITE device address.");

      SendAcks(m_i2cdev);
      outbuf=Read(m_i2cdev, data.size());
      for(uint32_t i=0;i<data.size();i++)
	data[i]=outbuf[i];

      SendNacks(m_i2cdev);
      Read(m_i2cdev, 1); // dummy read to send the NACK
    }
  else
    throw ComIOException("I2CFTDICom did not see an ACK.");

  if(Stop(m_i2cdev)!=0) 
    throw ComIOException("I2CFTDICom cannot send STOP.");
}
