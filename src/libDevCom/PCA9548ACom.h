#ifndef PCA9548ACOM_H
#define PCA9548ACOM_H

#include <memory>

#include "I2CCom.h"

class PCA9548ACom : public I2CCom
{
public:
  PCA9548ACom(uint8_t deviceAddr, uint8_t channel, std::shared_ptr<I2CCom> com);
  virtual ~PCA9548ACom();

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
  std::shared_ptr<I2CCom> m_com;
  uint8_t m_muxAddr;
  uint8_t m_channel;
};

#endif // PCA9548ACOM_H
