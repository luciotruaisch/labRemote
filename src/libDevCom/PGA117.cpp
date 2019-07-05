#include "PGA117.h"

#define RD 27136
#define WR 42

PGA117::PGA117(std::shared_ptr<SPIDevCom> device): m_dev(device)
{}

PGA117::~PGA117()
{}

void PGA117::write(uint8_t CH, uint8_t Gain)
{
  //Channel security
  if (CH > 9)
    {
      CH = 0;
    }

  //Set the gain measurement
  uint8_t setGain;
  switch(Gain)
    {
    case 1:
      setGain = 0;
    case 2:
      setGain = 1;
    case 5:
      setGain = 2;
    case 10:
      setGain = 3;
    case 20:
      setGain = 4;
    case 50:
      setGain = 5;
    case 100:
      setGain = 6;
    case 200:
      setGain = 7;
    default:
      setGain = 0;
    }

  //Set the write command to send
  uint8_t wr_command = WR;
  
  //Set the channel and the gain to send
  uint8_t data = ((0xF&setGain)<<4)|(0xF&CH); 

  //Sent signal
  m_dev->write_reg8(wr_command,data);
}

void PGA117::read(uint8_t& ch, uint8_t& gain)
{
  //Read the loaded value from device
  uint16_t data = m_dev->read_reg16(RD);

  //Channel activated
  ch = (uint8_t)(data&0xF);

  //Gain selected
  uint8_t rd_gain = ((data&0xF0)>>4);

  switch(rd_gain)
    {
    case 0:
      gain = 1;
    case 1:
      gain = 2;
    case 2:
      gain = 5;
    case 3:
      gain = 10;
    case 4:
      gain = 20;
    case 5:
      gain = 50;
    case 6:
      gain = 100;
    case 7:
      gain = 200;
    default:
      gain = 1;
    }
}
