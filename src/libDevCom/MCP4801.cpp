#include "MCP4801.h"

#include "LinearCalibration.h"

#include <iostream>
#include <iomanip>

MCP4801::MCP4801(std::shared_ptr<SPICom> com)
  : DACDevice(std::make_shared<LinearCalibration>(2.048, 0xFF)), m_com(com)
{ }

MCP4801::~MCP4801()
{ }

void MCP4801::setCount(uint32_t counts)
{
  counts=((counts<<4)|0x3000)&0xFFFF;
  m_com->write_reg16(counts);
}

void MCP4801::setCount(uint8_t ch, uint32_t counts)
{
  // TODO: Throw exception if wrong channel
  setCount(counts);
}

void MCP4801::setCount(const std::vector<uint8_t>& chs, const std::vector<uint32_t>& counts)
{
  for(uint8_t i=0; i<chs.size()-1; i++)
    {
      // TODO: Throw exception if wrong channel
      setCount(chs[i],counts[1]);
    }
}

uint32_t MCP4801::readCount()
{ return 0; }
 
uint32_t MCP4801::readCount(uint8_t ch)
{ return 0; }
 
void MCP4801::readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data)
{
  data.clear();
  data.resize(chs.size(), 0);
}

