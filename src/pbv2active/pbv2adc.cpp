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

int main()
{
  try
    {
      std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

      std::shared_ptr<I2CCom> adc_pwr_com=std::make_shared<PCA9548ACom>(0x22, 2, mux0_com);
      std::shared_ptr<I2CCom> adc_lv0_com=std::make_shared<PCA9548ACom>(0x21, 1, mux0_com);
      std::shared_ptr<I2CCom> adc_lv1_com=std::make_shared<PCA9548ACom>(0x22, 1, mux0_com);

      //
      // Test the power monitoring ADC
      std::cout << "----- Power Monitoring ADC -----" << std::endl;

      std::vector<uint8_t> result(8);
      adc_pwr_com->read_block(0xF0, result);
  
      for(int chi=0;chi<4;chi++)
	{
	  std::cout << "Read index " << chi << " = 0x" << std::hex
		    << std::setw(2) << std::setfill('0') << (uint)result[2*chi+0]
		    << std::setw(2) << std::setfill('0') << (uint)result[2*chi+1]
		    << std::dec << std::endl;
	  uint chrest=(result[2*chi+0]<<8)|(result[2*chi+1]<<0);
	  uint ch    =((chrest>>12)&0x007);
	  uint counts=((chrest>> 2)&0x3ff);
      
	  std::cout << "\tchannel = " << ch << std::endl;
	  std::cout << "\treading  = " << (double)counts/1024*3.3 << " V" << std::endl;
	}

      //
      // Test the LV monitoring ADC 0
      std::cout << "----- LV Monitoring ADC 0 -----" << std::endl;

      adc_lv0_com->write_reg16(0x2, 0x00F8);

      result.resize(16);      
      adc_lv0_com->read_block(0x70, result);
  
      for(int chi=0;chi<8;chi++)
	{
	  std::cout << "Read index " << chi << " = 0x" << std::hex
		    << std::setw(2) << std::setfill('0') << (uint)result[2*chi+0]
		    << std::setw(2) << std::setfill('0') << (uint)result[2*chi+1]
		    << std::dec << std::endl;
	  uint chrest=(result[2*chi+0]<<8)|(result[2*chi+1]<<0);
	  uint ch    =((chrest>>12)&0x007);
	  uint counts=((chrest>> 2)&0x3ff);
      
	  std::cout << "\tchannel = " << ch << std::endl;
	  std::cout << "\treading  = " << (double)counts/1024*3.3 << " V" << std::endl;
	}

      //
      // Test the LV monitoring ADC 1
      std::cout << "----- LV Monitoring ADC 1 -----" << std::endl;
      
      adc_lv1_com->write_reg16(0x2, 0x0FF8);

      result.resize(16);      
      adc_lv1_com->read_block(0x70, result);
  
      for(int chi=0;chi<8;chi++)
	{
	  std::cout << "Read index " << chi << " = 0x" << std::hex
		    << std::setw(2) << std::setfill('0') << (uint)result[2*chi+0]
		    << std::setw(2) << std::setfill('0') << (uint)result[2*chi+1]
		    << std::dec << std::endl;
	  uint chrest=(result[2*chi+0]<<8)|(result[2*chi+1]<<0);
	  uint ch    =((chrest>>12)&0x007);
	  uint counts=((chrest>> 2)&0x3ff);
      
	  std::cout << "\tchannel = " << ch << std::endl;
	  std::cout << "\treading  = " << (double)counts/1024*3.3 << " V" << std::endl;
	}
    }
  catch(ComIOException &e)
    {
      std::cout << e.what() << std::endl;
    }

  // std::cout << "All good" << std::endl;
  
  return 0;
}
