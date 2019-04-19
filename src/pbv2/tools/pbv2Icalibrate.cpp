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

#include "Bk85xx.h"
#include "SorensenPs.h"

loglevel_e loglevel = logINFO;

int main(int argc, char* argv[])
{
	if (argc < 3){
        std::cout << "Not enough parameters!" << std::endl;
        std::cout << "Usage: " << argv[0] << " <TESTNAME> <BK85XX>" << std::endl;
        return -1;
    }

    std::string filename = argv[1];
    std::string bkDev = argv[2];

	try
	{
		PBv2TB tb;

		std::fstream logfile(filename, std::fstream::out);
		//logfile << "Vin" << " " << "ADCVin" << " " << "ADCVinCurr" << "\n"
        //<< std::endl;
        logfile << "Vin" << " " << "bkDevVin" << " " << "ADCVin" << " " << "PSCurr" << " " << "BKCurr" <<  " " << "ADCCurr" << "\n" << std::endl;

        Bk85xx dc(bkDev);
        dc.setRemote();
        dc.setRemoteSense(false);
        dc.setModeCC();
        dc.setCurrent(0);
        dc.turnOn();
        logger(logINFO) << "DC Load turned on";
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
        std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

        //three adc
        std::shared_ptr<AD799X> adc_pwr=std::make_shared<AD799X>(3.3, AD799X::AD7993, std::make_shared<PCA9548ACom>(0x22, 2, mux0_com));
        std::shared_ptr<AD799X> adc_lv0=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x21, 1, mux0_com));
        std::shared_ptr<AD799X> adc_lv1=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x22, 1, mux0_com));
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        
        for(double i = 0; i<=2000; i+=50)
        {
            dc.setCurrent(i);

      		std::this_thread::sleep_for(std::chrono::milliseconds(500));
      	
            logfile << ps.getVoltage() << " ";
            logfile << dc.getValues().vol << " ";
            logfile << tb.getVin() << " ";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            logfile << ps.getCurrent() << " ";
            logfile << dc.getCurrent() << " ";
            logfile << tb.getVinCurrent() << "\n";
        }
	}

	catch(ComIOException &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
