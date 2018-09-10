#include "EndeavourRaw.h"

EndeavourRaw::EndeavourRaw(std::shared_ptr<DeviceCom> fpgaCom)
  : m_fpgaCom(fpgaCom)
{
  // Initialize default values (Note: FPGA clock = 100MHz, AMACv2 clock = 40MHz)
  setDitMin(  6*100./40.);
  setDitMid( 14*100./40.);
  setDitMax( 22*100./40.);

  setDahMin( 29*100./40.);
  setDahMid( 76*100./40.);
  setDahMax(124*100./40.);

  setBitGapMin( 11*100./40.);
  setBitGapMid( 43*100./40.);
  setBitGapMax( 75*100./40.);
  
}

void EndeavourRaw::setDitMin(uint DIT_MIN)
{
  uint val=m_fpgaCom->read_reg32(7);
  val=(val&0xFFFFFF00)|((DIT_MIN&0x0FF)<< 0);
  m_fpgaCom->write_reg32(7, val);
}

uint EndeavourRaw::getDitMin()
{ return (m_fpgaCom->read_reg32(7)>> 0)&0x0FF; }

void EndeavourRaw::setDitMid(uint DIT_MID)
{
  uint val=m_fpgaCom->read_reg32(7);
  val=(val&0xFFF000FF)|((DIT_MID&0xFFF)<< 8);
  m_fpgaCom->write_reg32(7, val);
}

uint EndeavourRaw::getDitMid()
{ return (m_fpgaCom->read_reg32(7)>> 8)&0xFFF; }

void EndeavourRaw::setDitMax(uint DIT_MAX)
{
  uint val=m_fpgaCom->read_reg32(7);
  val=(val&0x000FFFFF)|((DIT_MAX&0xFFF)<<20);
  m_fpgaCom->write_reg32(7, val);
}

uint EndeavourRaw::getDitMax()
{ return (m_fpgaCom->read_reg32(7)>>16)&0xFFF; }

void EndeavourRaw::setDahMin(uint DAH_MIN)
{
  uint val=m_fpgaCom->read_reg32(8);
  val=(val&0xFFFFFF00)|((DAH_MIN&0x0FF)<< 0);
  m_fpgaCom->write_reg32(8, val);
}

uint EndeavourRaw::getDahMin()
{ return (m_fpgaCom->read_reg32(8)>> 0)&0x0FF; }

void EndeavourRaw::setDahMid(uint DAH_MID)
{
  uint val=m_fpgaCom->read_reg32(8);
  val=(val&0xFFF000FF)|((DAH_MID&0xFFF)<< 8);
  m_fpgaCom->write_reg32(8, val);
}

uint EndeavourRaw::getDahMid()
{ return (m_fpgaCom->read_reg32(8)>> 8)&0xFFF; }

void EndeavourRaw::setDahMax(uint DAH_MAX)
{
  uint val=m_fpgaCom->read_reg32(8);
  val=(val&0x000FFFFF)|((DAH_MAX&0xFFF)<<20);
  m_fpgaCom->write_reg32(8, val);
}

uint EndeavourRaw::getDahMax()
{ return (m_fpgaCom->read_reg32(8)>>16)&0xFFF; }

void EndeavourRaw::setBitGapMin(uint BITGAP_MIN)
{
  uint val=m_fpgaCom->read_reg32(9);
  val=(val&0xFFFFFF00)|((BITGAP_MIN&0x0FF)<< 0);
  m_fpgaCom->write_reg32(9, val);
}

uint EndeavourRaw::getBitGapMin()
{ return (m_fpgaCom->read_reg32(9)>> 0)&0x0FF; }

void EndeavourRaw::setBitGapMid(uint BITGAP_MID)
{
  uint val=m_fpgaCom->read_reg32(9);
  val=(val&0xFFF000FF)|((BITGAP_MID&0xFFF)<< 8);
  m_fpgaCom->write_reg32(9, val);
}

uint EndeavourRaw::getBitGapMid()
{ return (m_fpgaCom->read_reg32(9)>> 8)&0xFFF; }

void EndeavourRaw::setBitGapMax(uint BITGAP_MAX)
{
  uint val=m_fpgaCom->read_reg32(9);
  val=(val&0x000FFFFF)|((BITGAP_MAX&0xFFF)<<20);
  m_fpgaCom->write_reg32(9, val);
}

uint EndeavourRaw::getBitGapMax()
{ return (m_fpgaCom->read_reg32(9)>>16)&0xFFF; }

void EndeavourRaw::reset()
{ m_fpgaCom->write_reg32(0, 0x1); }

bool EndeavourRaw::isError()
{ return (m_fpgaCom->read_reg32(0)>>2)&1; }

bool EndeavourRaw::isDataValid()
{
  return (m_fpgaCom->read_reg32(0)>>1)&1;
}

void EndeavourRaw::sendData(unsigned long long int data, unsigned int size)
{
  m_fpgaCom->write_reg32(1, size);
  m_fpgaCom->write_reg32(2, (data>>0 )&0xFFFFFFFF);
  m_fpgaCom->write_reg32(3, (data>>32)&0xFFFFFFFF);
  m_fpgaCom->write_reg32(0, 0x2);
}

void EndeavourRaw::readData(unsigned long long int& data, unsigned int& size)
{
  if(isDataValid())
    {
      size=m_fpgaCom->read_reg32(4);
      data=m_fpgaCom->read_reg32(6);
      data<<=32;
      data|=m_fpgaCom->read_reg32(5);
    }
  else
    {
      size=0;
      data=0;
    }
}
