#ifndef LTC2333_H
#define LTC2333_H

#include "ADCDevice.h"
#include "DeviceCom.h"
#include "UIOCom.h"
#include <vector>
#include <iostream>
#include <memory>
#include <unistd.h>

struct LTC2333Outputs{
  uint16_t result;
  uint8_t chan;
  uint8_t span;
};

class LTC2333 : public ADCDevice
{
public:
  LTC2333(std::shared_ptr<DeviceCom> dev);
  ~LTC2333();

  void init();
  void reset();

  // Sets next conversion result for specified channel and span, and
  //
  // Return value is a vector of length three, containing:
  // index 0: 18-bit conversion result
  // index 1: 3-bit channel ID
  // index 2: 3-bit soft span value
  //
  // Valid channels are 0-7.
  //
  // Valid spans are:
  // 
  // 0: 0V to 1.25*V_refbuf/1.024
  // 1: 0V to 1.25*V_refbuf
  // 2: +/-1.25*V_refbuf/1.024
  // 3: +/-1.25*V_refbuf
  // 4: 0V to 2.5*V_refbuf/1.024
  // 5: 0V to 2.5*V_refbuf
  // 6: +/-2.5*V_refbuf/1.024
  // 7: +/-2.5*V_refbuf
  //
  // inputSettings is a pair of (channel, span)
  void setADC(std::vector<std::pair<uint8_t,uint8_t>> inputSettings);

  // Reads out previous conversion result
  std::vector<LTC2333Outputs> getADC();

  // Calls previous two functions for a single channel, with default span of 0 
  // (but can set it as an option). 
  //
  // **Note that currently only a single channel works for setADC anyway!!
  LTC2333Outputs setAndReadChan(uint8_t chan, uint8_t span = 0);
  
private:
  std::shared_ptr<DeviceCom> m_dio	= std::make_shared<UIOCom>("/dev/uio0", 0x10000);
  std::shared_ptr<DeviceCom> m_dev;
  bool m_init = false;
  uint8_t m_nBytesIn;
  std::vector<std::pair<uint8_t,uint8_t>> m_inputSettings;

  const unsigned int m_chanMax = 0x7;
  const unsigned int m_spanMax = 0x7;

  // 3 bytes = 24 bits, which is enough for our ADC reads
  const unsigned int m_nBytesData = 3;

  bool checkValidInput(unsigned int chan, unsigned int span);
  unsigned int formatCommand(unsigned int chan, unsigned int span);
  std::vector<unsigned int> formatConversionResult(uint8_t* data);

};

#endif // LTC2333_H
