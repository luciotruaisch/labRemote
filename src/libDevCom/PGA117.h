#ifndef PGA117_H
#define PGA117_H

#include <stdint.h>
#include <memory>

#include "SPIDevCom.h"

class PGA117 
{
public:

  PGA117(std::shared_ptr<SPIDevCom> device);

  virtual ~PGA117();

  //Write a command to device
  void write(uint8_t CH, uint8_t Gain);

  //Read the loaded command from device
  void read(uint8_t& ch, uint8_t& gain);

private:
  
  //Pointer to SPI
  std::shared_ptr<SPIDevCom> m_dev;

};

#endif
