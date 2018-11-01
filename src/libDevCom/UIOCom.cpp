#include "UIOCom.h"

#include "ComIOException.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

UIOCom::UIOCom(const std::string& device, unsigned int size)
  : m_size(size)
{
  m_fd = open(device.c_str(), O_RDWR);
  if(m_fd==-1)
    throw ComIOException("UIOCom cannot open "+device+": "+strerror(errno));

  m_ptr = (unsigned*)mmap(NULL, m_size, PROT_READ|PROT_WRITE, MAP_SHARED, m_fd, 0);
}

UIOCom::~UIOCom()
{
  munmap(m_ptr, m_size);
}


void UIOCom::write_reg32(unsigned int address, unsigned int data)
{ m_ptr[address] = data; }

unsigned int UIOCom::read_reg32(unsigned int address)
{ return m_ptr[address]; }
