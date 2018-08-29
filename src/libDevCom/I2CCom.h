#ifndef I2CCOM_H
#define I2CCOM_H

#include "DeviceCom.h"

#include <string>
#include <vector>

class I2CCom : public DeviceCom
{
public:
  I2CCom(uint8_t deviceAddr);
  virtual ~I2CCom();

  void deviceAddr(uint8_t deviceAddr);
  uint8_t deviceAddr() const;

private:
  uint8_t m_deviceAddr;
};

#endif // I2CCOM_H
