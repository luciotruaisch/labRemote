#ifndef I2CDEVCOM_H
#define I2CDEVCOM_H

#include "I2CCom.h"

class I2CDevCom : public I2CCom
{
public:
  I2CDevCom(uint8_t deviceAddr, const std::string& i2cdev);
  virtual ~I2CDevCom();

  //
  // Write commands
  virtual void write_reg32(uint32_t address, uint32_t data);
  virtual void write_reg16(uint32_t address, uint16_t data);
  virtual void write_reg8 (uint32_t address, uint8_t  data);

  virtual void write_reg32(uint32_t data);
  virtual void write_reg16(uint16_t data);
  virtual void write_reg8 (uint8_t  data);

  virtual void write_block(uint32_t address, const std::vector<uint8_t>& data);
  virtual void write_block(const std::vector<uint8_t>& data);

  //
  // Read commands
  virtual uint32_t read_reg32(uint32_t address);
  virtual uint16_t read_reg16(uint32_t address);
  virtual uint8_t  read_reg8 (uint32_t address);

  virtual uint32_t read_reg32();
  virtual uint16_t read_reg16();
  virtual uint8_t  read_reg8 ();

  virtual void read_block(uint32_t address, std::vector<uint8_t>& data);
  virtual void read_block(std::vector<uint8_t>& data);
  
private:
  int m_fh =0;
};

#endif // I2CDEVCOM_H
