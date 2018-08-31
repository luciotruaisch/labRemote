#include "I2CDevCom.h"

#include "ComIOException.h"

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <cstring>
#include <string>
#include <iterator>

I2CDevCom::I2CDevCom(uint8_t deviceAddr, const std::string& i2cdev)
  : I2CCom(deviceAddr)
{
  m_fh = open(i2cdev.c_str(), O_RDWR);
  if(m_fh<0)
    throw ComIOException(std::string("I2CDev open failed: ")+std::strerror(errno));
}

I2CDevCom::~I2CDevCom()
{ }

void I2CDevCom::write_reg32(uint32_t address, uint32_t data)
{
  uint8_t inbuf[5] = {static_cast<uint8_t>(address&0xFF),
		      static_cast<uint8_t>((data>>24)&0xFF),
		      static_cast<uint8_t>((data>>16)&0xFF),
		      static_cast<uint8_t>((data>> 8)&0xFF),
		      static_cast<uint8_t>((data>> 0)&0xFF)};
  struct i2c_msg msgs[1];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = 0;
  msgs[0].len = 5;
  msgs[0].buf = inbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 1;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev write_reg32 failed: ")+std::strerror(errno));
}

void I2CDevCom::write_reg16(uint32_t address, uint16_t data)
{
  uint8_t inbuf[3] = {static_cast<uint8_t>(address&0xFF),
		      static_cast<uint8_t>((data>> 8)&0xFF),
		      static_cast<uint8_t>((data>> 0)&0xFF)};
  struct i2c_msg msgs[1];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = 0;
  msgs[0].len = 3;
  msgs[0].buf = inbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 1;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev write_reg16 failed: ")+std::strerror(errno));
}

void I2CDevCom::write_reg8 (uint32_t address, uint8_t  data)
{
  uint8_t inbuf[2] = {static_cast<uint8_t>(address&0xFF),
		      data};
  struct i2c_msg msgs[1];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = 0;
  msgs[0].len = 2;
  msgs[0].buf = inbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 1;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev write_reg8 failed: ")+std::strerror(errno));

}

void I2CDevCom::write_reg32(uint32_t data)
{
  write_block({static_cast<uint8_t>((data>>24)&0xFF),
	static_cast<uint8_t>((data>>16)&0xFF),
	static_cast<uint8_t>((data>> 8)&0xFF),
	static_cast<uint8_t>((data>> 0)&0xFF)});
}

void I2CDevCom::write_reg16(uint16_t data)
{
  write_block({static_cast<uint8_t>((data>> 8)&0xFF),
	static_cast<uint8_t>((data>> 0)&0xFF)});
}

void I2CDevCom::write_reg8 (uint8_t  data)
{
  write_block({data});
}

void I2CDevCom::write_block(uint32_t address, const std::vector<uint8_t>& data)
{
  std::vector<uint8_t> inbuf=data;
  inbuf.insert(inbuf.begin(),static_cast<uint8_t>(address&0xFF));
  struct i2c_msg msgs[1];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = 0;
  msgs[0].len = inbuf.size();
  msgs[0].buf = &inbuf[0];

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 1;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev write_block failed: ")+std::strerror(errno));

}

void I2CDevCom::write_block(const std::vector<uint8_t>& data)
{
  std::vector<uint8_t> inbuf=data;

  struct i2c_msg msgs[1];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = 0;
  msgs[0].len = inbuf.size();
  msgs[0].buf = &inbuf[0];

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 1;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev write_block failed: ")+std::strerror(errno));
}

uint32_t I2CDevCom::read_reg32(uint32_t address)
{
  uint8_t outbuf[4];
  uint8_t inbuf[1]={static_cast<uint8_t>(address&0xFF)};
  struct i2c_msg msgs[2];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = 0;
  msgs[0].len = 1;
  msgs[0].buf = inbuf;

  msgs[1].addr = deviceAddr();
  msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
  msgs[1].len = 4;
  msgs[1].buf = outbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 2;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev read_reg32 failed: ")+std::strerror(errno));

  return (outbuf[0]<< 24)|(outbuf[1]<< 16)|(outbuf[2]<< 8)|(outbuf[3]<< 0);
}

uint16_t I2CDevCom::read_reg16(uint32_t address)
{
  uint8_t outbuf[2];
  uint8_t inbuf[1]={static_cast<uint8_t>(address&0xFF)};
  struct i2c_msg msgs[2];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = 0;
  msgs[0].len = 1;
  msgs[0].buf = inbuf;

  msgs[1].addr = deviceAddr();
  msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
  msgs[1].len = 2;
  msgs[1].buf = outbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 2;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev read_reg16 failed: ")+std::strerror(errno));

  return (outbuf[0]<< 8)|(outbuf[1]<< 0);
}

uint8_t  I2CDevCom::read_reg8 (uint32_t address)
{
  uint8_t outbuf[1];
  uint8_t inbuf[1]={static_cast<uint8_t>(address&0xFF)};
  struct i2c_msg msgs[2];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = 0;
  msgs[0].len = 1;
  msgs[0].buf = inbuf;

  msgs[1].addr = deviceAddr();
  msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
  msgs[1].len = 1;
  msgs[1].buf = outbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 2;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev read_reg8 failed: ")+std::strerror(errno));

  return outbuf[0];
}

uint32_t I2CDevCom::read_reg32()
{
  uint8_t outbuf[4];
  struct i2c_msg msgs[1];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[1].addr = deviceAddr();
  msgs[1].flags = I2C_M_RD;
  msgs[1].len = 4;
  msgs[1].buf = outbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 1;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev read_reg32 failed: ")+std::strerror(errno));

  return (outbuf[0]<< 24)|(outbuf[1]<< 16)|(outbuf[2]<< 8)|(outbuf[3]<< 0);
}

uint16_t I2CDevCom::read_reg16()
{
  uint8_t outbuf[2];
  struct i2c_msg msgs[1];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[1].addr = deviceAddr();
  msgs[1].flags = I2C_M_RD;
  msgs[1].len = 2;
  msgs[1].buf = outbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 1;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev read_reg32 failed: ")+std::strerror(errno));

  return (outbuf[0]<< 8)|(outbuf[1]<< 0);
}

uint8_t  I2CDevCom::read_reg8 ()
{
  uint8_t outbuf[1];
  struct i2c_msg msgs[1];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[1].addr = deviceAddr();
  msgs[1].flags = I2C_M_RD;
  msgs[1].len = 1;
  msgs[1].buf = outbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 1;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev read_reg32 failed: ")+std::strerror(errno));

  return outbuf[0];
}

void I2CDevCom::read_block(uint32_t address, std::vector<uint8_t>& data)
{
  uint8_t inbuf[1]={static_cast<uint8_t>(address&0xFF)};
  struct i2c_msg msgs[2];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = 0;
  msgs[0].len = 1;
  msgs[0].buf = inbuf;

  msgs[1].addr = deviceAddr();
  msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
  msgs[1].len = data.size();
  msgs[1].buf = &data[0];

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 2;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev read_block failed: ")+std::strerror(errno));
}

void I2CDevCom::read_block(std::vector<uint8_t>& data)
{
  struct i2c_msg msgs[1];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = deviceAddr();
  msgs[0].flags = I2C_M_RD;
  msgs[0].len = data.size();
  msgs[0].buf = &data[0];

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 1;

  if (ioctl(m_fh, I2C_RDWR, &msgset) < 0)
    throw ComIOException(std::string("I2CDev read_block failed: ")+std::strerror(errno));
}
