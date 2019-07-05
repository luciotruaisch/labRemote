#include "SPIDevCom.h"

#include "ComIOException.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <iostream>
#include <errno.h>
#include <thread>

#ifndef __LEMON__
 
#include <linux/spi/spidev.h>
#include <cstring>
#include <string>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

SPIDevCom::SPIDevCom(SPICom::eMode SPIMode, SPICom::eBits SPISign, uint8_t WORD_SIZE, const std::string& spidev):SPICom(SPIMode, SPISign, WORD_SIZE)
{
  //Open the device file
  m_fh = open(spidev.c_str(), O_RDWR);
  if(m_fh<0)
    {
      throw ComIOException(std::string("SPIDev open failed: ")+std::strerror(errno));
    }


  //Define the SPI transfer Mode
  int spimode;
  switch(getSPIMode())
    {
    case mode_0:
      spimode = SPI_MODE_0;
      break;
    case mode_1:
      spimode = SPI_MODE_1;
      break;
    case mode_2:
      spimode = SPI_MODE_2;
      break;
    case mode_3:
      spimode = SPI_MODE_3;
      break;
    default:
      spimode = SPI_MODE_0;
      break;
    }

  if(ioctl(m_fh,SPI_IOC_WR_MODE,&spimode)==-1)
    {
      throw ComIOException("SPICom can't set mode");
    }

  //Define Bit justification
  enum SPICom::eBits LSB = LSB_first;
  uint8_t SPI_bit = 0;
  if(SPISign == LSB)
    {
      SPI_bit = 8;
    }

  if(ioctl(m_fh,SPI_IOC_WR_LSB_FIRST, &SPI_bit)==-1)
    {
      throw ComIOException("SPICom can't set bit justification");
    }

  //Define Word Size
  if(ioctl(m_fh,SPI_IOC_WR_BITS_PER_WORD, &WORD_SIZE)==-1)
    {
      throw ComIOException("SPICom can't set word size");
    }

  //Define MaxSpeed
  uint32_t maxSpeed = 10000000;
  if(ioctl(m_fh,SPI_IOC_WR_MAX_SPEED_HZ, &maxSpeed)==-1)
    {
      throw ComIOException("SPICom can't set max Speed");
    }
}

SPIDevCom::~SPIDevCom()
{
  close(m_fh);
}

void SPIDevCom::write_reg8(uint8_t data)
{
  //Upload the message
  uint8_t tx[] = {data};
  uint8_t rx[ARRAY_SIZE(tx)] = {0, };
  struct spi_ioc_transfer msgs = {
    .tx_buf = (unsigned long)tx,
    .rx_buf = (unsigned long)rx,
    .len = ARRAY_SIZE(tx)};


  //Write to SPI device
  if(ioctl(m_fh,SPI_IOC_MESSAGE(1),&msgs)<1)
    {
      std::cout << strerror(errno) << std::endl;
      throw ComIOException("SPICom can't write");
    }     
}

void SPIDevCom::write_reg8(uint32_t address, uint8_t  data)
{
  //Upload the message
  uint8_t tx[] = {(uint8_t)(address&0xFF),(data)};
  uint8_t rx[ARRAY_SIZE(tx)] = {0, };
  struct spi_ioc_transfer msgs = {
    .tx_buf = (unsigned long)tx,
    .rx_buf = (unsigned long)rx,
    .len = ARRAY_SIZE(tx)};

  //Write to SPI device
  if(ioctl(m_fh,SPI_IOC_MESSAGE(1),&msgs)<1)
    {
      std::cout << strerror(errno) << std::endl;
      throw ComIOException("SPICom can't write");
    }     
}

uint16_t SPIDevCom::read_reg16(uint32_t address)
{
  //Part the sent data
  uint8_t tx_1 = (uint8_t)((0xFF00&address)>>8);
  uint8_t tx_2 = (uint8_t)(0xFF&address);

  //Write the read command
  uint8_t sent_tx[]={tx_1,tx_2};
  uint8_t sent_rx[ARRAY_SIZE(sent_tx)] = {0, };
  struct spi_ioc_transfer sent_msgs = {
    .tx_buf = (unsigned long)sent_tx,
    .rx_buf = (unsigned long)sent_rx,
    .len = ARRAY_SIZE(sent_tx)};

  if(ioctl(m_fh,SPI_IOC_MESSAGE(1),&sent_msgs)<1)
    {
      std::cout << strerror(errno) << std::endl;
      throw ComIOException("SPICom can't write");
    }  

  //Read messages
  uint8_t read_tx[]={0,0};
  uint8_t read_rx[ARRAY_SIZE(read_tx)] = {0, };
  struct spi_ioc_transfer read_msgs = {
    .tx_buf = (unsigned long)read_tx,
    .rx_buf = (unsigned long)read_rx,
    .len = ARRAY_SIZE(read_tx)};

  if(ioctl(m_fh,SPI_IOC_MESSAGE(1),&read_msgs)<1)
    {
      std::cout << strerror(errno) << std::endl;
      throw ComIOException("SPICom can't write");
    }  

  //Read from rx buffer 
  uint16_t result = read_rx[0]<<8|read_rx[1];
  return result;
}

void SPIDevCom::write_reg16(uint16_t data)
{}

void SPIDevCom::write_block(const std::vector<uint8_t>& data)
{}

uint16_t SPIDevCom::read_reg16()
{return 0;}

 void SPIDevCom::read_block(std::vector<uint8_t>& data)
 {}

 void SPIDevCom::write_reg32(uint32_t address, uint32_t data)
 {}
 void SPIDevCom::write_reg16(uint32_t address, uint16_t data)
 {}
 void SPIDevCom::write_reg32(uint32_t data)
 {}
 void SPIDevCom::write_block(uint32_t address, const std::vector<uint8_t>& data)
 {}
 uint32_t SPIDevCom::read_reg32(uint32_t address)
 {return 0;}

 uint8_t  SPIDevCom::read_reg8 (uint32_t address)
 {return 0;}
 uint32_t SPIDevCom::read_reg32()
 {return 0;}
 uint8_t  SPIDevCom::read_reg8()
 {return 0;}
 void SPIDevCom::read_block(uint32_t address, std::vector<uint8_t>& data)
 {}

#endif
