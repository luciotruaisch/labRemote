#include "MAX11619.h"

#include "LinearCalibration.h"

#include "NotSupportedException.h"

MAX11619::MAX11619(double reference, std::shared_ptr<SPICom> com)
  : ADCDevice(std::make_shared<LinearCalibration>(reference, 0x3FF)),
    m_com(com)
{ }

MAX11619::~MAX11619()
{ }

uint32_t MAX11619::readCount()
{
  m_com->write_block({0x18,0xF9});

  return (m_com->read_reg16(16)>>2)&0x3FF;
}

uint32_t MAX11619::readCount(uint8_t ch)
{
  m_com->write_block({0x18,0xF9});

  std::vector<uint8_t> data(2*4);
  m_com->read_block(data);
  uint16_t chvalue=(data[2*ch+0]<<8)|(data[2*ch+1]<<0);
  
  return (chvalue>>2)&0x3FF;
}

void MAX11619::readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& counts)
{
  m_com->write_block({0x18,0xF9});

  std::vector<uint8_t> data(2*4);
  m_com->read_block(data);

  counts.clear();
  for(uint8_t chi=0;chi<chs.size();chi++)
    {
      uint16_t chresult=(data[2*chi+0]<<8)|(data[2*chi+1]<<0);
      counts.push_back((chresult>>2)&0x3FF);
    }
}
