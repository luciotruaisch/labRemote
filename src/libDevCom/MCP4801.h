#ifndef MCP4801_H
#define MCP4801_H

#include <memory>

#include "SPICom.h"
#include "DACDevice.h"

class MCP4801 : public DACDevice
{
public:
  MCP4801(std::shared_ptr<SPICom> com);
  virtual ~MCP4801();

  virtual void setCount(uint32_t counts);
  virtual void setCount(uint8_t ch, uint32_t counts);
  virtual void setCount(const std::vector<uint8_t>& chs, const std::vector<uint32_t>& counts);

  virtual uint32_t readCount();
  virtual uint32_t readCount(uint8_t ch);
  virtual void readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data);

private:
  std::shared_ptr<SPICom> m_com;
};

#endif // MCP4801_H
