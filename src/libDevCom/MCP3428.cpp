#include "MCP3428.h"
#include <iostream>
#include "GainCalibration.h"
#include "NotSupportedException.h"


MCP3428::MCP3428(double reference, nr_bit bit_res, conversion conv_mode, gain_PGA PGA, std::shared_ptr<I2CCom> com): ADCDevice(std::make_shared<GainCalibration>(reference,(uint8_t)bit_res,(uint8_t)PGA)), m_bit_res(bit_res), m_conv_mode(conv_mode), m_PGA(PGA), m_com(com)
{
}

MCP3428::~MCP3428()
{}

//Default read on device on ch0
uint32_t  MCP3428::readCount()
{
  //Resolution bit
  uint8_t bit;
  switch(m_bit_res)
    {
    case e12_bit:
      bit = 0;
      break;
    case e14_bit:
      bit = 1;
      break;
    case e16_bit:
      bit = 2;
      break;
    default:
      bit = 0;
      break;
    }

  //Configuration register
  uint8_t vConfig = 0x80|(m_conv_mode << 4)|(bit << 2)|m_PGA;

  //Write on configuration register
  m_com->write_reg8(vConfig);

  //Read the regsiter
  std::vector<uint8_t>data(3);

  //Wait until the read value is not ready
  bool ready = 0;
  while(ready != 1)
    {
      m_com->read_block(0x7F&vConfig,data);

      //Test if the conversion result is ready
      if ((data[2]&0x80) == 0x00)
	{
	  ready = 1;
	}
    }


  uint16_t chresult = (data[0]<<8)|(data[1]<<0);
  uint32_t chcount;

  //Output signal
  switch (m_bit_res)
    {
    case e12_bit:
      chcount = 0x0FFF & chresult;
      break;
    case e14_bit:
      chcount = 0x3FFF & chresult;
      break;
    case e16_bit:
      chcount = 0xFFFF & chresult;
      break;
    default:
      throw NotSupportedException("Model not supported by MCP3428");
      break;
    }

  return chcount;
}

uint32_t MCP3428::readCount(uint8_t ch)
{
  uint32_t chcount = 0;

  //Check if Channel exist
  if (ch < m_numChannels)
    {
      //Resolution bit
      uint8_t bit;
      switch(m_bit_res)
	{
	case e12_bit:
	  bit = 0;
	  break;
	case e14_bit:
	  bit = 1;
	  break;
	case e16_bit:
	  bit = 2;
	  break;
	default:
	  bit = 0;
	  break;
	}

      //Configuration register
      uint8_t vConfig = 0;
      vConfig = 0x80|(ch<<5)|(m_conv_mode << 4)|(bit << 2)|m_PGA;

      //Write on configuration register
      m_com->write_reg8(vConfig);
      
      //Read the regsiter
      std::vector<uint8_t>data(3);

      //Wait until the read value is not ready
      bool ready = 0;
      while(ready != 1)
	{
	  m_com->read_block(0x7F&vConfig,data);

	  //Test if the conversion result is ready
	  if ((data[2]&0x80) == 0x00)
	    {
	      ready = 1;
	    }
	}

      uint16_t chresult = (data[0]<<8)|(data[1]<<0);

      //Output signal
      switch (m_bit_res)
	{
	case e12_bit:
	  chcount = 0x0FFF & chresult;
	  break;
	case e14_bit:
	  chcount = 0x3FFF & chresult;
	  break;
	case e16_bit:
	  chcount = 0xFFFF & chresult;
	  break;
	default:
	  throw NotSupportedException("Model not supported by MCP3428");
	  break;
	}
    }else
    {
      throw NotSupportedException("Model not supported by MCP3428");
    }
  return chcount;
}

void MCP3428::readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& counts)
{
  //Clear counts table
  counts.clear();

  //Data table
  std::vector<uint8_t>data(2);

  //Do the Voltage measurement sequence
  for(uint8_t i=0; i<chs.size(); i++)
    {
       //Check if Channel exist
       if (chs[i] < m_numChannels)
        {
          //Resolution bit
	  uint8_t bit;
	  switch(m_bit_res)
	    {
	    case e12_bit:
	      bit = 0;
	      break;
	    case e14_bit:
	      bit = 1;
	      break;
	    case e16_bit:
	      bit = 2;
	      break;
	    default:
	      bit = 0;
	      break;
	    }

	  //Configuration register
          uint8_t vConfig = 0x80|(chs[i]<<5)|(m_conv_mode << 4)|(bit << 2)|m_PGA;
          //Write on configuration register
          m_com->write_reg8(vConfig);

	  //Read the regsiter
	  std::vector<uint8_t>data(3);

	  //Wait until the read value is not ready
	  bool ready = 0;
	  while(ready != 1)
	    {
	      m_com->read_block(0x7F&vConfig,data);

	      //Test if the conversion result is ready
	      if ((data[2]&0x80) == 0x00)
		{
		  ready = 1;
		}
	    }

          uint16_t chresult = (data[0]<<8)|(data[1]<<0);

          //Output signal
          switch (m_bit_res)
	  {
	  case e12_bit:
	    counts[i] = 0x0FFF & chresult;
	    break;
	  case e14_bit:
	    counts[i] = 0x3FFF & chresult;
	    break;
	  case e16_bit:
	    counts[i] = 0xFFFF & chresult;
	    break;
	  default:
	    throw NotSupportedException("Model not supported by MCP3428");
	    break;
	  }
         }else
         {
          throw NotSupportedException("Model not supported by MCP3428");
         }
    }    
}

