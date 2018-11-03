#ifndef MCP3428_H
#define MCP3428_H

#include <stdint.h>

#include <memory>
//#include <map>

#include "I2CCom.h"
#include "ADCDevice.h"

class MCP3428 : public ADCDevice
{
public:

  //Bit resolution
  enum nr_bit {e12_bit = 12, e14_bit = 14, e16_bit = 16};

  //Conversion mode
  enum conversion {eShot = 0, eCont = 1};

  //Gain
  enum gain_PGA {e_x1 = 0, e_x2 = 1, e_x4 = 2, e_x8 = 3};

  //Initialisation of ADC
  MCP3428(double reference, nr_bit bit_res, conversion  conv_mode, gain_PGA  PGA,  std::shared_ptr<I2CCom> com);
  virtual ~MCP3428();

  //Read value from ADC
  virtual uint32_t readCount();
  virtual uint32_t readCount(uint8_t ch);
  virtual void 	 readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data);

private:

  //Properties of device
  const  uint8_t m_numChannels = 4;
  nr_bit m_bit_res;
  conversion  m_conv_mode;
  gain_PGA m_PGA;

  //Pointer for I2C communication
  std::shared_ptr<I2CCom> m_com;
};
	




#endif // MCP3428_H
