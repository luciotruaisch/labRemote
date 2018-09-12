#include "EndeavourCom.h"

#include "EndeavourComException.h"
#include "EndeavourRawUIO.h"

#include <iostream>
#include <memory>
#include <unistd.h>

#define TIMEOUT 100

EndeavourCom::EndeavourCom(unsigned short amacid, std::unique_ptr<EndeavourRaw> raw)
  : m_amacid(amacid&0x1F)
{
  m_raw=std::move(raw);
}

const std::unique_ptr<EndeavourRaw>& EndeavourCom::raw()
{ return m_raw; }

void EndeavourCom::enableSeqNum(bool enableSeqNum)
{ m_enableSeqNum=enableSeqNum; }

void EndeavourCom::reset()
{
  m_raw->reset();
  m_seqnum=0;
}

void EndeavourCom::setid(REFMODE mode, unsigned int refid)
{
  // Want to send 1's 1000 times, to avoid hysteresis issues
  // where the voltages on the n and p sides of CMDIN are swapped.
  // With ~15 foot cables, we needed ~500 1's for this to work
  // properly, so let's send 1000 of them to be safe!
  for(unsigned int iter = 0; iter < 20; ++iter){
    m_raw->sendData(0x3ffffffffffff,50);
    usleep(1);
  }

  // to give space between our hysteresis 1's and the actual SETID
  usleep(10);

  unsigned long long int databits=0;

  //{3'b110, 5'b11111, 3'b111, newamacid[4:0], 4'b1111, efuseid[19:0], 3'b111, idpads[4:0], crc[7:0]}

  // SETID - 3'b110
  databits<<=3;
  databits|=0b110; 

  // 3'b11111
  databits<<=5;
  databits|=0b11111;

  // 3'b111
  databits<<=3;
  databits|=0b111;

  // newamacid[4:0]
  databits<<=5;
  databits|=(m_amacid&0x1F);

  // 4'b1111
  databits<<=4;
  databits|=0b1111;

  // efuseid[19:0]
  databits<<=20;
  if(mode==REFMODE::EfuseId)
    databits|=(refid&0xFFFFF);
  else
    databits|=0xFFFFF;

  // 3'b111
  databits<<=3;
  databits|=0b111;

  // idpads[4:0]
  databits<<=5;
  if(mode==IDPads)
    databits|=(refid&0x1F);
  else
    databits|=0x1F;

  // crc[7:0]
  unsigned int crc=calc_crc(databits);
  databits<<=8;
  databits|=(crc&0xFF);

  // send data
  m_raw->sendData(databits, 56);
  m_seqnum++;

  // wait for response ( poll :( )
  uint tryidx=0;
  for(tryidx=0;tryidx<TIMEOUT;tryidx++)
    {
      usleep(10);
      if(m_raw->isDataValid()) break;
    }
  if(tryidx==TIMEOUT)
    throw EndeavourComException("SETID timeout");

  // Parse the response
  unsigned long long int read_data;
  unsigned int read_nbits;
  m_raw->readData(read_data,read_nbits);
  //std::cout << m_raw->isDataValid() << " " << read_nbits << " " << std::hex << read_data << std::dec << std::endl;

  if(read_nbits!=8)
    throw EndeavourComException("SETID recieved wrong number of bits: %d (expected 8)",read_nbits);

  unsigned short int seqnum=read_data&0b111;
  if(m_enableSeqNum && seqnum!=m_seqnum)
    throw EndeavourComException("SETID recieved wrong sequence: %d (expected %d)",seqnum,m_seqnum);

  unsigned short int amacid=(read_data>>3)&0b11111;
  if(amacid!=m_amacid)
    throw EndeavourComException("SETID recieved wrong amacid: %d (expected %d)",amacid,m_amacid);
}

void EndeavourCom::write_reg(unsigned int address, unsigned int data)
{
  unsigned long long int databits=0;

  //{3'b111, amacid[4:0], addr[7:0], data[31:0], crc[7:0]}

  // WRITE - 3'b110
  databits<<=3;
  databits|=0b111; 

  // amacid[4:0]
  databits<<=5;
  databits|=(m_amacid&0x1F);

  // addr[7:0]
  databits<<=8;
  databits|=(address&0xFF);

  // data[31:0]
  databits<<=32;
  databits|=(data&0xFFFFFFFF);

  // crc[7:0]
  unsigned int crc=calc_crc(databits);
  databits<<=8;
  databits|=(crc&0xFF);

  // send data
  m_raw->sendData(databits, 56);

  // wait for response ( poll :( )
  uint tryidx=0;
  for(tryidx=0;tryidx<TIMEOUT;tryidx++)
    {
      usleep(10);
      if(m_raw->isDataValid()) break;
    }
  if(tryidx==TIMEOUT)
    throw EndeavourComException("WRITE timeout");

  // Parse the response
  unsigned long long int read_data;
  unsigned int read_nbits;
  m_raw->readData(read_data,read_nbits);
  //std::cout << m_raw->isDataValid() << " " << read_nbits << " " << std::hex << read_data << std::dec << std::endl;
  m_seqnum++;

  if(read_nbits!=8)
    throw EndeavourComException("WRITE recieved wrong number of bits: %d (expected 8)",read_nbits);

  unsigned short int seqnum=read_data&0b111;
  if(m_enableSeqNum && seqnum!=m_seqnum)
    throw EndeavourComException("WRITE recieved wrong sequence: %d (expected %d)",seqnum,m_seqnum);

  unsigned short int amacid=(read_data>>3)&0b11111;
  if(amacid!=m_amacid)
    throw EndeavourComException("WRITE recieved wrong amacid: %d (expected %d)",amacid,m_amacid);
}

unsigned int EndeavourCom::read_reg(unsigned int address)
{
  unsigned long long int databits=0;

  //{3'b101, amacid[4:0], addr[7:0]}

  // READ - 3'b101
  databits<<=3;
  databits|=0b101;

  // amacid[4:0]
  databits<<=5;
  databits|=(m_amacid&0x1F);

  // addr[7:0]
  databits<<=8;
  databits|=(address&0xFF);

  // send data
  m_raw->sendData(databits, 16);

  // wait for response ( poll :( )
  uint tryidx=0;
  for(tryidx=0;tryidx<TIMEOUT;tryidx++)
    {
      usleep(10);
      if(m_raw->isDataValid()) break;
    }
  if(tryidx==TIMEOUT)
    throw EndeavourComException("READ timeout");
  //std::cout << "Recieved READ response after " << tryidx+1 << " tries." << std::endl;  

  // Parse the response
  unsigned long long int read_data;
  unsigned int read_nbits;
  m_raw->readData(read_data,read_nbits);
  //std::cout << m_raw->isDataValid() << " " << read_nbits << " " << std::hex << read_data << std::dec << std::endl;
  m_seqnum++;

  if(read_nbits!=48)
    throw EndeavourComException("READ recieved wrong number of bits: %d (expected 48)",read_nbits);

  unsigned int data=read_data&0xFFFFFFFF;

  unsigned short int retaddress=(read_data>>32)&0xFF;
  if(retaddress!=address)
    throw EndeavourComException("READ recieved wrong address: 0x%08X (expected %08X)",retaddress,address);

  unsigned short int seqnum=(read_data>>40)&0b111;
  if(m_enableSeqNum && seqnum!=m_seqnum)
    throw EndeavourComException("READ recieved wrong sequence: %d (expected %d)",seqnum,m_seqnum);

  unsigned short int amacid=(read_data>>43)&0b11111;
  if(amacid!=m_amacid)
    throw EndeavourComException("READ recieved wrong amacid: %d (expected %d)",amacid,m_amacid);

  return data;
}

unsigned int EndeavourCom::readnext_reg()
{
  unsigned long long int databits=0;

  //{3'b100, amacid[4:0]}

  // READNEXT - 3'b100
  databits<<=3;
  databits|=0b100;

  // amacid[4:0]
  databits<<=5;
  databits|=(m_amacid&0x1F);

  // send data
  m_raw->sendData(databits, 8);

  // wait for response ( poll :( )
  uint tryidx=0;
  for(tryidx=0;tryidx<TIMEOUT;tryidx++)
    {
      usleep(10);
      if(m_raw->isDataValid()) break;
    }
  if(tryidx==TIMEOUT)
    throw EndeavourComException("READNEXT timeout");

  // Parse the response
  unsigned long long int read_data;
  unsigned int read_nbits;
  m_raw->readData(read_data,read_nbits);
  //std::cout << m_raw->isDataValid() << " " << read_nbits << " " << std::hex << read_data << std::dec << std::endl;
  m_seqnum++;

  if(read_nbits!=48)
    throw EndeavourComException("READNEXT recieved wrong number of bits: %d (expected 48)",read_nbits);

  unsigned int data=read_data&0xFFFFFFFF;

  //unsigned short int retaddress=(read_data>>32)&0xFF;

  unsigned short int seqnum=(read_data>>40)&0b111;
  if(m_enableSeqNum && seqnum!=m_seqnum)
    throw EndeavourComException("READNEXT recieved wrong sequence: %d (expected %d)",seqnum,m_seqnum);

  unsigned short int amacid=(read_data>>43)&0b11111;
  if(amacid!=m_amacid)
    throw EndeavourComException("READ recieved wrong amacid: %d (expected %d)",amacid,m_amacid);

  return data;
}

unsigned int EndeavourCom::calc_crc(unsigned long long int data) const
{
  unsigned crc=0;
  for(unsigned int i=0;i<8;i++)
    {
      crc|=(( ((data>>(8*0+i))&1) ^
	      ((data>>(8*1+i))&1) ^
	      ((data>>(8*2+i))&1) ^
	      ((data>>(8*3+i))&1) ^
	      ((data>>(8*4+i))&1) ^
	      ((data>>(8*5+i))&1) )<<i);
    }

  return crc;
}
