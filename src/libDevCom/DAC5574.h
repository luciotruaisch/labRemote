#ifndef DAC5574_H
#define DAC5574_H

#include <memory>

#include "I2CCom.h"
#include "DACDevice.h"

class DAC5574 : public DACDevice
{
public:
  DAC5574(float reference, std::shared_ptr<I2CCom> com);
  virtual ~DAC5574();

  virtual void setCount(uint32_t counts);
  virtual void setCount(uint8_t ch, uint32_t counts);
  virtual void setCount(const std::vector<uint8_t>& chs, const std::vector<uint32_t>& counts);

  virtual uint32_t readCount();
  virtual uint32_t readCount(uint8_t ch);
  virtual void readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data);

private:
  std::shared_ptr<I2CCom> m_com;
};

#endif // DAC5574_H
