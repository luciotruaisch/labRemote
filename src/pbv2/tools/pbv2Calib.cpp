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

//make calibration data
//set each power supply to step through possible values
//read out each dac and compare to what it is supposed to be
//read out each input from the power supplies for comparison
//going to need the inputs for both power supplies

loglevel_e loglevel = logINFO;

int main(int argc, char* argv[])
{
    if (argc < 2){
        logger(logERROR) << "Not enough parameters!";
        return -1;
    }

    std::string filename = argv[1];
    //try to do things  
    try
    {
        //set power supply
        //change to match
        //SorensenPs ps1("/dev/ttyUSB0", 11);
       
        //ps1.init();
        //ps1.setVoltage(1.5);
        //ps1.setCurrent(3);
        //std::this_thread::sleep_for(std::chrono::seconds(1));

        //ps1.turnOn();
        //ps1.turnOff();
        //Sorensen ps2("/dev/ttyUSB0", 11);
       
        //ps2.init();
        //ps2.setVoltage(11);
        //ps2.setCurrent(6);

        //ps2.turnOn();
        std::cout << "this is working" << std::endl;

        std::fstream logfile(filename, std::fstream::out);
        //logfile << "Current Voltage" << std::endl;

        //read data from power supply 
        //std::string curr = ps1.getCurrent();
        //std::string volt = ps1.getVoltage();
        //std::cout << curr << volt << std::endl;
        //logfile << curr << volt << std::endl;
        //put dac readings here? 
        
        //get data from PBv2TB    
        PBv2TB tb;

        std::cout << "P5VCurrent" << "\t" << "M5VCurrent" << "\t" << "Vin" << "\t"
        << "VinCurrent" << std::endl;
        //logfile << "P5VCurrent" << "\t" << "M5VCurrent" << "\t" << "Vin" << "\t"
        //<< "VinCurrent" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        //print out TB readings
        std::cout << tb.getP5VCurrent() << "\t" << tb.getM5VCurrent() << "\t"
        << tb.getVin() << "\t" << tb.getVinCurrent() << std::endl;

        //logfile << tb.getP5VCurrent() << "\t" << tb.getM5VCurrent() << "\t"
        //<< tb.getVin() << "\t" << tb.getVinCurrent() << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        

        //read out the DAC values
        //I2C instance mux0
        std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

        //three dac outputs 
        //three adc
        std::shared_ptr<DAC5574> dac0=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4C, 1, mux0_com));
        std::shared_ptr<DAC5574> dac1=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4D, 1, mux0_com));
        std::shared_ptr<DAC5574> dac2=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4E, 1, mux0_com));
        std::shared_ptr<AD799X> adc_pwr=std::make_shared<AD799X>(3.3, AD799X::AD7993, std::make_shared<PCA9548ACom>(0x22, 2, mux0_com));
        std::shared_ptr<AD799X> adc_lv0=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x21, 1, mux0_com));
        std::shared_ptr<AD799X> adc_lv1=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x22, 1, mux0_com));

        
        //set all dac values
        dac0->set(0, 0.1);
        dac0->set(1, 0.2);
        dac0->set(2, 0.3);
        dac0->set(3, 0.4);

        dac1->set(0, 0.5);
        dac1->set(1, 0.6);
        dac1->set(2, 0.7);
        dac1->set(3, 0.8);

        dac2->set(0, 0.9);
        dac2->set(1, 1.0);
        dac2->set(2, 1.1);
        dac2->set(3, 1.2);

       

        //logfile << "dac0 channel 0 set to 0.1" << "\t" << "dac0 channel 1 set to 0.1" << "\t"
        //<< "dac0 channel 2 set to 0.3" << "\t" << "dac0 channel 4 set to 0.4" << std::endl;
        //logfile << "dac1 channel 0 set to 0.5" << "\t" << "dac1 channel 1 set to 0.6" << "\t"
        //<< "dac1 channel 2 set to 0.7" << "\t" << "dac1 channel 4 set to 0.8" << std::endl;
        //logfile << "dac2 channel 0 set to 0.9" << "\t" << "dac2 channel 1 set to 1.0" << "\t"
        //<< "dac2 channel 2 set to 1.1" << "\t" << "dac2 channel 4 set to 1.2" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "----- LV DAC 0 -----" << std::endl;
        //logfile << "----- LV DAC 0 -----" << std::endl;

        std::cout << "Set CH0 to " << dac0->read(0) << " V" << std::endl;
        std::cout << "Set CH1 to " << dac0->read(1) << " V" << std::endl;
        std::cout << "Set CH2 to " << dac0->read(2) << " V" << std::endl;
        std::cout << "Set CH3 to " << dac0->read(3) << " V" << std::endl;
        //logfile << dac0->read(0) << std::endl;
        //logfile << dac0->read(1) << std::endl;
        //logfile << dac0->read(2) << std::endl;
        //logfile << dac0->read(3) << std::endl;

        std::cout << "----- LV DAC 1 -----" << std::endl;

        std::cout << "Set CH0 to " << dac1->read(0) << " V" << std::endl;
        std::cout << "Set CH1 to " << dac1->read(1) << " V" << std::endl;
        std::cout << "Set CH2 to " << dac1->read(2) << " V" << std::endl;
        std::cout << "Set CH3 to " << dac1->read(3) << " V" << std::endl;
        //logfile << dac1->read(0) << std::endl;
        //logfile << dac1->read(1) << std::endl;
        //logfile << dac1->read(2) << std::endl;
        //logfile << dac1->read(3) << std::endl;

        std::cout << "----- LV DAC 2 -----" << std::endl;

        std::cout << "Set CH0 to " << dac2->read(0) << " V" << std::endl;
        std::cout << "Set CH1 to " << dac2->read(1) << " V" << std::endl;
        std::cout << "Set CH2 to " << dac2->read(2) << " V" << std::endl;
        std::cout << "Set CH3 to " << dac2->read(3) << " V" << std::endl;
        //logfile << dac2->read(0) << std::endl;
        //logfile << dac2->read(1) << std::endl;
        //logfile << dac2->read(2) << std::endl;
        //logfile << dac2->read(3) << std::endl;

        
        std::cout << "----- Clear All -----" << std::endl;
        dac0->set(0, 0);
        dac0->set(1, 0);
        dac0->set(2, 0);
        dac0->set(3, 0);
        dac1->set(0, 0);
        dac1->set(1, 0);
        dac1->set(2, 0);
        dac1->set(3, 0);
        dac2->set(0, 0);
        dac2->set(1, 0);
        dac2->set(2, 0);
        dac2->set(3, 0);


        std::vector<double> volts;

        logfile << "P5VCurrent" << " " << "Power0" << " " << "Power1" << " "<< "Power2"<< " " << "Power3"<< " " <<
        "ADC00" << " " << "ADC01"<< " " << "ADC02"<< " " << "ADC03"<< " " << "ADC04"<< " " << "ADC05"<< " " << "ADC06"<< " " << "ADC07"<< " " <<
        "ADC10" << " " << "ADC11"<< " " << "ADC12"<< " " << "ADC13"<< " " << "ADC14"<< " " << "ADC15"<< " " << "ADC16"<< " " << "ADC17" << std::endl;

        // Test the power monitoring ADC
        std::cout << "----- Power Monitoring ADC -----" << std::endl;
        //logfile << "----- Power Monitoring ADC -----" << std::endl;
        logfile << tb.getP5VCurrent() << " ";
        adc_pwr->read({0,1,2,3}, volts);
  
        for(int ch=0;ch<4;ch++){
            std::cout << "ch " << ch << " = "<< volts[ch] << " V" << std::endl;
            logfile << volts[ch] << " ";
        }
      
        // Test the LV monitoring ADC 0
        std::cout << "----- LV Monitoring ADC 0 -----" << std::endl;
        //logfile << "----- LV Monitoring ADC 0-----" << std::endl;


        adc_lv0->read({0,1,2,3,4,5,6,7}, volts);
  
        for(int ch=0;ch<8;ch++){
            std::cout << "ch " << ch << " = "<< volts[ch] << " V" << std::endl;
            logfile << volts[ch] << " ";
        }

      
        // Test the LV monitoring ADC 1
        std::cout << "----- LV Monitoring ADC 1 -----" << std::endl;
        //logfile << "----- LV Monitoring ADC 1 -----" << std::endl;

        adc_lv1->read({0,1,2,3,4,5,6,7}, volts);
  
        for(int ch=0;ch<8;ch++){
            std::cout << "ch " << ch << " = "<< volts[ch] << " V" << std::endl;
            logfile << volts[ch] << " ";
        }


        //ps1.turnOff();

    }

    catch(ComIOException &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;

}
