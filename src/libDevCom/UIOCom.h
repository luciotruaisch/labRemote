#ifndef UIOCOM_H
#define UIOCOM_H

#include <string>

#include "DeviceCom.h"

class UIOCom : public DeviceCom
{
public:
  UIOCom(const std::string& device, unsigned int size);
  virtual ~UIOCom();

  virtual void write_reg32(unsigned int address, unsigned int data);
  virtual unsigned int read_reg32(unsigned int address);
	
private:
  unsigned int m_size;

  int m_fd;
  unsigned *m_ptr;
};

#endif // UICOM_H
