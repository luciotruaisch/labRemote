#include "DAC5574.h"

#include "LinearCalibration.h"

#include <iostream>
#include <iomanip>

DAC5574::DAC5574(float reference, std::shared_ptr<I2CCom> com)
  : DACDevice(std::make_shared<LinearCalibration>(reference, 0xFF)), m_com(com)
{ }

DAC5574::~DAC5574()
{ }

void DAC5574::setCount(uint32_t counts)
{
  m_com->write_block({static_cast<uint8_t>((counts>>0)&0xFF),
	0x00});
}

void DAC5574::setCount(uint8_t ch, uint32_t counts)
{
  uint8_t controlbyte=0x10|(ch<<1);
  m_com->write_block({controlbyte,
	static_cast<uint8_t>((counts>>0)&0xFF),
	0x00});
}

void DAC5574::setCount(const std::vector<uint8_t>& chs, const std::vector<uint32_t>& counts)
{
  if(chs.size()==1)
    { // only one
      setCount(chs[0], counts[0]);
    }
  else
    { // more than one, set only temp register for N-1 and then set all when setting N

      for(uint8_t i=0; i<chs.size()-1; i++)
	{
	  uint8_t controlbyte=0x00|(chs[i]<<1);
	  m_com->write_block({controlbyte,
	    static_cast<uint8_t>((counts[i]>>8)&0xFF), 
	    static_cast<uint8_t>((counts[i]>>0)&0xFF)});
	}

      uint8_t controlbyte=0x02|(chs.back()<<1);
      m_com->write_block({controlbyte,
	    static_cast<uint8_t>((counts.back()>>8)&0xFF), 
	    static_cast<uint8_t>((counts.back()>>0)&0xFF)});
    }
}

uint32_t DAC5574::readCount()
{
  return m_com->read_reg16()>>8;
}
 
uint32_t DAC5574::readCount(uint8_t ch)
{
  return m_com->read_reg16(ch<<1)>>8;
}
 
void DAC5574::readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data)
{
  data.clear();
  for(uint8_t ch : chs)
    data.push_back(readCount(ch));
}

