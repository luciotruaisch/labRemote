#ifndef SPICOM_H
#define SPICOM_H

#include "DeviceCom.h"

#include <string>
#include <vector>

class SPICom : public DeviceCom
{
 public:
  enum eMode {mode_0, mode_1, mode_2, mode_3};
  enum eBits {MSB_first, LSB_first};

  SPICom(SPICom::eMode SPI_mode, SPICom::eBits sign_BIT, uint8_t WORD_SIZE);
  virtual ~SPICom();

  //Get attributes
  eMode getSPIMode();
  eBits getSPISign();
  uint8_t getWORDSize();

  //Set attributes
  void setSPIMode(eMode newMode);
  void setSPISign(eBits newSign);
  void setWORDSize(uint8_t newSize);

 protected:
  //Specify the SPI transfer Mode
 enum eMode m_MODE;
  //Specify if the transmission is LSB or MSB first
 enum eBits m_Bits;
  //Specify how many bits are in one word
  uint8_t m_WORD_SIZE;
  
};

#endif // SPICOM_H
