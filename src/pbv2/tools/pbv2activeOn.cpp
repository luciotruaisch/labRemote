#include <memory>
#include <iostream>
#include <fstream>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>

#include "I2CDevCom.h"
#include "PCA9548ACom.h"
#include "PBv2TB.h"
#include "ComIOException.h"
#include "DAC5574.h"
#include "Logger.h"
#include "AD799X.h"

#include "SorensenPs.h"

loglevel_e loglevel = logINFO;
//code to turn on all dacs to a non-zero level and hold the value for 600 seconds for probing 
int main(int argc, char* argv[])
{
  if (argc < 1)
    {
      std::cout << "Not enough parameters!" << std::endl;
      std::cout << "Parameters needed: <PBNumber>" << std::endl;
      return -1;
    }

  //int pb = strtol(argv[1], NULL, 10);

  try
    {
      //PBv2TB tb;

      //int DACNum = tb.getDAC_Num(pb);
      //int CHVALUE = tb.getDAC_CH(pb);

      //std::cout << DACNum << " " << CHVALUE << std::endl;

      //if(DACNum == -1 || CHVALUE == -1)
      //{
      //	std::cout << "PB number must be an interger between 1-9" << std::endl;
      //	return -1;
      //}

      //initiate power supply 
      SorensenPs ps("/dev/ttyUSB0", 11);

      ps.setCh(2);
      ps.init();
      ps.setVoltage(11);
      ps.setCurrent(5);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      ps.turnOn();
      std::cout << "Power supply turned on, voltage set to " << ps.getVoltage() <<
	" current set to " << ps.getCurrent();

      //I2C instance mux0
      //std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

      //three dac outputs 
      //three adc
      //std::shared_ptr<DAC5574> dac0=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4C, 1, mux0_com));
      //std::shared_ptr<DAC5574> dac1=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4D, 1, mux0_com));
      //std::shared_ptr<DAC5574> dac2=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4E, 1, mux0_com));
      //std::shared_ptr<AD799X> adc_pwr=std::make_shared<AD799X>(3.3, AD799X::AD7993, std::make_shared<PCA9548ACom>(0x22, 2, mux0_com));
      //std::shared_ptr<AD799X> adc_lv0=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x21, 1, mux0_com));
      //std::shared_ptr<AD799X> adc_lv1=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x22, 1, mux0_com));


      //tb.setDac(DACNum, CHVALUE, 1.0);
      /*
        dac0->set(0, 1.0);
        dac0->set(1, 1.0);
        dac0->set(2, 1.0);
        dac0->set(3, 1.0);
        dac1->set(0, 1.0);
        dac1->set(1, 1.0);
        dac1->set(2, 1.0);
        dac1->set(3, 1.0);
        dac2->set(0, 1.0);
        dac2->set(1, 1.0);
        dac2->set(2, 1.0);
        dac2->set(3, 1.0);

      	std::cout << "DAC set to " << tb.readDac(DACNum, CHVALUE) << " V" << std::endl;
      */
      std::this_thread::sleep_for(std::chrono::seconds(1));
      double curr = ps.getCurrent();

      //std::vector<double> volts;
      //adc_pwr->read({0,1,2,3}, volts);

      ps.turnOff();
    }
  catch(ComIOException &e)
    {
      std::cout << e.what() << std::endl;
    }

  return 0;
}
