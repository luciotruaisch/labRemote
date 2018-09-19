#include "PBv3CommPatchSPICom.h"

#include "ftdihelp.h"

#include <libftdi1/ftdi.h>
#include <unistd.h>

#include <iostream>

PBv3CommPatchSPICom::PBv3CommPatchSPICom(struct ftdi_context *ftdi, uint8_t csbit)
  : m_ftdi(ftdi), m_csbit(csbit)
{ }

PBv3CommPatchSPICom::~PBv3CommPatchSPICom()
{ }

void PBv3CommPatchSPICom::write_reg32(uint32_t address, uint32_t data)
{
  write_reg32(address);
  write_block({static_cast<uint8_t>((data>>24)&0xFF),
	static_cast<uint8_t>((data>>16)&0xFF),
	static_cast<uint8_t>((data>> 8)&0xFF),
	static_cast<uint8_t>((data>> 0)&0xFF)});
}

void PBv3CommPatchSPICom::write_reg16(uint32_t address, uint16_t data)
{
  write_reg32(address);
  write_block({static_cast<uint8_t>((data>> 8)&0xFF),
	static_cast<uint8_t>((data>> 0)&0xFF)});
}

void PBv3CommPatchSPICom::write_reg8 (uint32_t address, uint8_t  data)
{
  write_reg32(address);
  write_block({static_cast<uint8_t>((data>> 0)&0xFF)});
}

void PBv3CommPatchSPICom::write_reg32(uint32_t data)
{
  write_block({static_cast<uint8_t>((data>>24)&0xFF),
	static_cast<uint8_t>((data>>16)&0xFF),
	static_cast<uint8_t>((data>> 8)&0xFF),
	static_cast<uint8_t>((data>> 0)&0xFF)});
}

void PBv3CommPatchSPICom::write_reg16(uint16_t data)
{
  write_block({static_cast<uint8_t>((data>> 8)&0xFF),
	static_cast<uint8_t>((data>> 0)&0xFF)});  
}

void PBv3CommPatchSPICom::write_reg8 (uint8_t  data)
{
  write_block({static_cast<uint8_t>((data>> 0)&0xFF)});  
}

void PBv3CommPatchSPICom::write_block(uint32_t address, const std::vector<uint8_t>& data)
{
  write_reg32(address);
  write_block(data);
}

void PBv3CommPatchSPICom::write_block(const std::vector<uint8_t>& data)
{
  ftdi_usb_purge_buffers(m_ftdi);

  std::vector<uint8_t> ftdidata;

  ftdidata={0x83};
  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
  ftdi_read_data(m_ftdi, &ftdidata[0], 1);
  uint8_t hbyte=ftdidata[0];
  
  // set CS low
  hbyte&=~(1<<m_csbit);
  ftdidata={0x82, hbyte, 0xDF};
  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());  

  // Let's do a data write
  for(uint32_t byte : data)
    {
      for(uint8_t nbit=8;nbit>0;nbit--)
  	{
	  bool bit=((byte>>(nbit-1))&1);

	  // clock low and load data
  	  hbyte&=0x7F;
  	  hbyte=(bit)?(hbyte|(1<<6)):(hbyte&~(1<<6));

  	  ftdidata={0x82, hbyte, 0xDF};
  	  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
  	  usleep(1e3);	  

  	  // clock high
  	  hbyte|=0x80;
  	  ftdidata={0x82, hbyte, 0xDF};
  	  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
  	  usleep(1e3);

  	  // clock low
  	  hbyte&=0x7F;
  	  ftdidata={0x82, hbyte, 0xDF};
  	  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
  	  usleep(1e3);	  
  	}
    }

  // set CS high
  hbyte|=(1<<m_csbit);
  ftdidata={0x82, hbyte, 0xDF};
  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());

}

uint32_t PBv3CommPatchSPICom::read_reg32(uint32_t address)
{
  write_reg32(address);
  return read_reg32();
}

uint16_t PBv3CommPatchSPICom::read_reg16(uint32_t address)
{
  write_reg32(address);
  return read_reg16();
}

uint8_t  PBv3CommPatchSPICom::read_reg8 (uint32_t address)
{
  write_reg32(address);
  return read_reg8();
}

uint32_t PBv3CommPatchSPICom::read_reg32()
{
  std::vector<uint8_t> data={0x00,0x00,0x00,0x00};
  read_block(data);
  return (data[0]<<24)|(data[1]<<16)|(data[2]<< 8)|(data[3]<< 0);
}

uint16_t PBv3CommPatchSPICom::read_reg16()
{
  std::vector<uint8_t> data={0x00,0x00};
  read_block(data);
  return (data[0]<< 8)|(data[1]<< 0);
}

uint8_t  PBv3CommPatchSPICom::read_reg8 ()
{
  std::vector<uint8_t> data={0x00};
  read_block(data);
  return data[0];
}

void PBv3CommPatchSPICom::read_block(uint32_t address, std::vector<uint8_t>& data)
{
  write_reg32(address);
  read_block(data);
}

void PBv3CommPatchSPICom::read_block(std::vector<uint8_t>& data)
{
  ftdi_usb_purge_buffers(m_ftdi);

  std::vector<uint8_t> ftdidata;

  ftdidata={0x83};
  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
  ftdi_read_alldata(m_ftdi, ftdidata, 1);
  uint8_t hbyte=ftdidata[0];
  
  // set CS low
  hbyte&=~(1<<m_csbit);
  ftdidata={0x82, hbyte, 0xDF};
  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());  

  // Let's do a data read
  for(uint ibyte=0;ibyte<data.size();ibyte++)
    {
      uint8_t byte=0;
      for(uint ibit=0;ibit<8;ibit++)
  	{
  	  // clock low
  	  hbyte&=0x7F;
  	  ftdidata={0x82, hbyte, 0xDF};
  	  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
	  usleep(1e3);

  	  // clock high
  	  hbyte|=0x80;
  	  ftdidata={0x82, hbyte, 0xDF};
  	  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
  	  usleep(1e3);

  	  // read bit
  	  ftdidata={0x83};
  	  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
	  ftdi_read_alldata(m_ftdi, ftdidata, 1);

  	  byte<<=1;	  
  	  byte|=((ftdidata[0]>>5)&1);

  	  // clock low
  	  hbyte&=0x7F;
  	  ftdidata={0x82, hbyte, 0xDF};
  	  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
	  //usleep(1e3);
  	}
      
      data[ibyte]=byte;
    }

  // set CS high
  hbyte|=(1<<m_csbit);
  ftdidata={0x82, hbyte, 0xDF};
  ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size());
}
