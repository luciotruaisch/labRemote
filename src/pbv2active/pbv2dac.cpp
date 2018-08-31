//
// Random tests for the active board

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>
#include <memory>

#include "I2CDevCom.h"
#include "PCA9548ACom.h"

#include "ComIOException.h"

#include "DAC5574.h"

int main()
{
  try
    {
      std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

      std::shared_ptr<DAC5574> dac0=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4C, 1, mux0_com));

      dac0->set(0, 0.1);
      dac0->set(1, 0.2);
      dac0->set(2, 0.3);
      dac0->set(3, 0.4);

      std::cout << "Set CH0 to " << dac0->read(0) << " V" << std::endl;
      std::cout << "Set CH1 to " << dac0->read(1) << " V" << std::endl;
      std::cout << "Set CH2 to " << dac0->read(2) << " V" << std::endl;
      std::cout << "Set CH3 to " << dac0->read(3) << " V" << std::endl;
    }
  catch(ComIOException &e)
    {
      std::cout << e.what() << std::endl;
    }

  // std::cout << "All good" << std::endl;
  
  return 0;
}
