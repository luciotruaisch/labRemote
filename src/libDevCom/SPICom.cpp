#include "SPICom.h"

SPICom::SPICom(SPICom::eMode SPI_mode, SPICom::eBits sign_BIT, uint8_t WORD_SIZE ):m_MODE(SPI_mode), m_Bits(sign_BIT), m_WORD_SIZE(WORD_SIZE)
{ }

SPICom::~SPICom()
{ }

enum SPICom::eMode SPICom::getSPIMode()
{
  return m_MODE;
}

enum SPICom::eBits SPICom::getSPISign()
{
  return m_Bits;
}

uint8_t SPICom::getWORDSize()
{
  return m_WORD_SIZE;
}

void SPICom::setSPIMode(eMode newMode)
{
  m_MODE = newMode;
}

void SPICom::setSPISign(eBits newSign)
{
  m_Bits = newSign;
}

void SPICom::setWORDSize(uint8_t newSize)
{
  m_WORD_SIZE = newSize;
}
