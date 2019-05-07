#ifndef MAX11619_H
#define MAX11619_H

#include <stdint.h>

#include <memory>
#include <map>

#include "SPICom.h"
#include "ADCDevice.h"

class MAX11619 : public ADCDevice
{
public:
  MAX11619(double reference, std::shared_ptr<SPICom> com);
  virtual ~MAX11619();

  virtual uint32_t readCount();
  virtual uint32_t readCount(uint8_t ch);
  virtual void     readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data);

private:
  std::shared_ptr<SPICom> m_com;
};

#endif // MAX11619_H
