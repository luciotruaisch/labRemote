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

#include "AD799X.h"
#include "MCP3428.h"
#include "Logger.h"
loglevel_e loglevel = logINFO;


int main()
{
  try
    {
      std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

      std::shared_ptr<AD799X> adc_pwr=std::make_shared<AD799X>(3.3, AD799X::AD7994, std::make_shared<PCA9548ACom>(0x22, 2, mux0_com));
      std::shared_ptr<AD799X> adc_lv0=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x21, 1, mux0_com));
      std::shared_ptr<AD799X> adc_lv1=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x22, 1, mux0_com));

      std::shared_ptr<MCP3428> adc_hv0=std::make_shared<MCP3428>(2.048, MCP3428::e16_bit, MCP3428::eShot, MCP3428::e_x1, std::make_shared<PCA9548ACom>(0x68, 2, mux0_com));
      std::shared_ptr<MCP3428> adc_hv1=std::make_shared<MCP3428>(2.048, MCP3428::e16_bit, MCP3428::eShot, MCP3428::e_x1, std::make_shared<PCA9548ACom>(0x6C, 2, mux0_com));
      std::shared_ptr<MCP3428> adc_hv2=std::make_shared<MCP3428>(2.048, MCP3428::e16_bit, MCP3428::eShot, MCP3428::e_x1, std::make_shared<PCA9548ACom>(0x6A, 2, mux0_com));

      std::vector<double> volts;

      //
      // Test the power monitoring ADC
      std::cout << "----- Power Monitoring ADC -----" << std::endl;

      adc_pwr->read({0,1,2,3}, volts);
  
      for(int ch=0;ch<4;ch++)
	std::cout << "ch " << ch << " = "
		  << volts[ch] << " V" << std::endl;

      //
      // Test the LV monitoring ADC 0
      std::cout << "----- LV Monitoring ADC 0 -----" << std::endl;

      adc_lv0->read({0,1,2,3,4,5,6,7}, volts);
  
      for(int ch=0;ch<8;ch++)
	std::cout << "ch " << ch << " = "
		  << volts[ch] << " V" << std::endl;      

      //
      // Test the LV monitoring ADC 1
      std::cout << "----- LV Monitoring ADC 1 -----" << std::endl;

      adc_lv1->read({0,1,2,3,4,5,6,7}, volts);
  
      for(int ch=0;ch<8;ch++)
	std::cout << "ch " << ch << " = "
		  << volts[ch] << " V" << std::endl;

      // Test the HV monitoring ADC 0
      std::cout << "----- HV  Monitoring ADC 0 -----" << std::endl;

      adc_hv0->read({0,1,2,3}, volts);
  
      for(int ch=0;ch<4;ch++)
	std::cout << "ch " << ch << " = "
		  << volts[ch] << " V" << std::endl;
    }
  catch(ComIOException &e)
    {
      std::cout << e.what() << std::endl;
    }

  // std::cout << "All good" << std::endl;
  
  return 0;
}
