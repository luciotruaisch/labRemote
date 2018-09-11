#include "EndeavourRawFTDI.h"

#include "EndeavourComException.h"

#include <libftdi1/ftdi.h>
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <stdint.h>

#define CHUNK_SIZE 65535
#define LATENCY_MS 2
#define USB_TIMEOUT 120000


EndeavourRawFTDI::EndeavourRawFTDI()
{
  int ret;
  if((m_ftdi=ftdi_new())==0)
    throw EndeavourComException("Unable to open FTDI");

  if((ret=ftdi_usb_open(m_ftdi,0x0403,0x6014))<0)
    throw EndeavourComException("Unable to find FTDI device: " + std::string(ftdi_get_error_string(m_ftdi)));


  //
  // Configure device for MPSSE
  
  // Reset USB device
  ftdi_usb_reset(m_ftdi);
  
  // Set USB request transfer sizes to 64K
  ftdi_write_data_set_chunksize(m_ftdi, CHUNK_SIZE);
  ftdi_read_data_set_chunksize (m_ftdi, CHUNK_SIZE);

  // Disable event and error characters
  ftdi_set_event_char(m_ftdi, 0, false);
  ftdi_set_error_char(m_ftdi, 0, false);

  // Sets the read and write timeouts in milliseconds
  m_ftdi->usb_read_timeout  = USB_TIMEOUT;
  m_ftdi->usb_write_timeout = USB_TIMEOUT;

  // Set the latency timer to 1mS (default is 16mS)
  ftdi_set_latency_timer(m_ftdi, LATENCY_MS);

  // Turn on flow control to synchronize IN requests
  ftdi_setflowctrl(m_ftdi, SIO_RTS_CTS_HS);

  // Reset controller
  ftdi_set_bitmode(m_ftdi, 0x0, BITMODE_RESET);

  // Enable MPSSE mode
  ftdi_set_bitmode(m_ftdi, 0x0, BITMODE_MPSSE);

  // Purge buffers
  ftdi_usb_purge_rx_buffer(m_ftdi);
  ftdi_usb_purge_tx_buffer(m_ftdi);


  std::vector<uint8_t> data={0x80, 0x00, 0xFB};
  ret=ftdi_write_data(m_ftdi, &data[0], data.size());
  std::cout << "Write 0x80, 0x00, 0xFB - Return " << ret << std::endl;

  data[0]=0x81;
  ret=ftdi_write_data(m_ftdi, &data[0], 1);
  std::cout << "Write 0x81 - Return " << ret << std::endl;

  ret=ftdi_read_data(m_ftdi, &data[0], 1);
  std::cout << "Read - Return " << ret << std::endl;
  std::cout << std::hex; for(uint8_t i=0;i<ret;i++) { std::cout << " " << (uint32_t)data[i]; } std::cout << std::endl;

  data={0x8A};
  ftdi_write_data(m_ftdi, &data[0], data.size());

  data={0x86, 0x00, 0x00};
  ftdi_write_data(m_ftdi, &data[0], data.size());
}

EndeavourRawFTDI::~EndeavourRawFTDI()
{
  ftdi_usb_close(m_ftdi);
  ftdi_free(m_ftdi);
}

void EndeavourRawFTDI::setDitMin(uint DIT_MIN)
{ m_DIT_MIN=DIT_MIN; }

uint EndeavourRawFTDI::getDitMin()
{ return m_DIT_MIN; }

void EndeavourRawFTDI::setDitMid(uint DIT_MID)
{ m_DIT_MID=DIT_MID; }

uint EndeavourRawFTDI::getDitMid()
{ return m_DIT_MID; }

void EndeavourRawFTDI::setDitMax(uint DIT_MAX)
{ m_DIT_MAX=DIT_MAX; }

uint EndeavourRawFTDI::getDitMax()
{ return m_DIT_MAX; }

void EndeavourRawFTDI::setDahMin(uint DAH_MIN)
{ m_DAH_MIN=DAH_MIN; }

uint EndeavourRawFTDI::getDahMin()
{ return m_DAH_MIN; }

void EndeavourRawFTDI::setDahMid(uint DAH_MID)
{ m_DAH_MID=DAH_MID; }

uint EndeavourRawFTDI::getDahMid()
{ return m_DAH_MID; }

void EndeavourRawFTDI::setDahMax(uint DAH_MAX)
{ m_DAH_MAX=DAH_MAX; }

uint EndeavourRawFTDI::getDahMax()
{ return m_DAH_MAX; }

void EndeavourRawFTDI::setBitGapMin(uint BITGAP_MIN)
{ m_BITGAP_MIN=BITGAP_MIN; }

uint EndeavourRawFTDI::getBitGapMin()
{ return m_BITGAP_MIN; }

void EndeavourRawFTDI::setBitGapMid(uint BITGAP_MID)
{ m_BITGAP_MID=BITGAP_MID; }

uint EndeavourRawFTDI::getBitGapMid()
{ return m_BITGAP_MID; }

void EndeavourRawFTDI::setBitGapMax(uint BITGAP_MAX)
{ m_BITGAP_MAX=BITGAP_MAX; }

uint EndeavourRawFTDI::getBitGapMax()
{ return m_BITGAP_MAX; }

void EndeavourRawFTDI::reset()
{  }

bool EndeavourRawFTDI::isError()
{ return false; }

bool EndeavourRawFTDI::isDataValid()
{ return false; }

void EndeavourRawFTDI::sendData(unsigned long long int data, unsigned int size)
{
  std::vector<uint8_t> ftdidata;
  
  uint8_t bytecounter=0, byte=0;
  for(uint32_t bit=size;bit>0;bit--)
    {
      bool value=(data>>(bit-1))&1;

      // Add 1's for data
      for(uint32_t i=0;i<((value)?m_DAH_MID:m_DIT_MID);i++)
      {
	byte<<=1;
	byte|=1;
	bytecounter++;
	if(bytecounter==8)
	  {
	    ftdidata.push_back(byte);
	    byte=0;
	    bytecounter=0;
	  }
      }

      // Add 0's for interbit gap
      for(uint32_t i=0;i<m_BITGAP_MID;i++)
	{
	  byte<<=1;
	  bytecounter++;
	  if(bytecounter==8)
	    {
	      ftdidata.push_back(byte);
	      byte=0;
	      bytecounter=0;
	    }
	}
    }

  // Pad rest of the bits
  if(bytecounter>0)
    {
      byte<<=(8-bytecounter);
      ftdidata.push_back(byte);
    }

  // Prepare send package
  uint16_t Length=ftdidata.size()-1;
  ftdidata.insert(ftdidata.begin(),0x10);
  ftdidata.insert(ftdidata.begin()+1,(Length>>0)&0xFF);
  ftdidata.insert(ftdidata.begin()+2,(Length>>8)&0xFF);

  std::cout << "sending" << std::hex << std::endl;
  for(const auto& x : ftdidata)
    std::cout << (uint32_t)x << std::endl;
  std::cout << std::dec;

  // Set the data
  std::cout << ftdi_write_data(m_ftdi, &ftdidata[0], ftdidata.size()) << std::endl;
  usleep(500e3);
  //std::cout << ftdi_read_data(m_ftdi, &ftdidata[0], ftdidata.size()) << std::endl;
  //std::cout << "reading" << std::hex << std::endl;
  //for(const auto& x : ftdidata)
  //std::cout << (uint32_t)x << std::endl;
  //std::cout << std::dec;
}

void EndeavourRawFTDI::readData(unsigned long long int& data, unsigned int& size)
{ }
