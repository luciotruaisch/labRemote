#include <string>
#include <iostream>
#include <thread>

#include "DeviceCom.h"
#include "SPICom.h"
#include "SPIDevCom.h"

int main()
{
  std::cout << "test SPI communication" << std::endl;

  SPIDevCom SPI_Device(SPICom::mode_0, SPICom::MSB_first, 0, "/dev/spidev1.0");

  while(1)
    {
      std::cout << "Sent signal = 38" << std::endl; 
      SPI_Device.write_reg8((uint32_t)42,(uint8_t)38);
      std::this_thread::sleep_for(std::chrono::seconds(10));
      std::cout << "Received signal =" << (unsigned int)SPI_Device.read_reg16(31232) << std::endl;
    }
}
