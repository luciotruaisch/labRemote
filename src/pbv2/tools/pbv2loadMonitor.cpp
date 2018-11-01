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

int main(int argc, char* argv[])
{
  if (argc < 3)
    {
      std::cout << "Not enough parameters!" << std::endl;
      std::cout << "Parameters needed: <PBidx> <filename>" << std::endl;
      return -1;
    }

  int pb = strtol(argv[1], NULL, 10);
  std::string filename = argv[2];

  try
    {
      PBv2TB tb;

      std::fstream logfile(filename, std::fstream::out);
      logfile << "run" << " " << "time" << " " << "DAC" << " " << "Iout"
	      << " " << "NTC_L1L3"
	      << " " << "NTC_L2L4"
	      << " " << "NTC_L3L5"
	      << " " << "NTC_L4L6"
	      << " " << "NTC_L5L7"
	      << " " << "NTC_L6L8"
	      << " " << "NTC_L7L9" << std::endl;

      //initiate power supply 
      SorensenPs ps("/dev/ttyUSB0", 11);
      //ps.setCh(0);

      ps.init();
      ps.setVoltage(1.5);
      ps.setCurrent(5);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      ps.turnOn();
      std::this_thread::sleep_for(std::chrono::seconds(1));      
      std::cout << "Power supply turned on, voltage set to " << ps.getVoltage()
		<< " current set to " << ps.getCurrent() << std::endl;

      uint32_t dac=0;
      uint32_t run=0;
      while(true)
        {
	  for(uint i=0;i<10+30*60;i++)
	    {
	      if(i==0)
		{
		  dac=0; // Cooldown
		  tb.setLoadCounts(pb,dac);
		}
	      else if(i==10)
		{
		  dac=170;
		  tb.setLoadCounts(pb,dac);
		}
	      
	      long int time=std::time(0);

	      logfile   << run << " " << time << " " << dac << " " << ps.getCurrent()
			<< " " << tb.getNTC(NTC_CH_L1L3)
			<< " " << tb.getNTC(NTC_CH_L2L4)
			<< " " << tb.getNTC(NTC_CH_L3L5)
			<< " " << tb.getNTC(NTC_CH_L4L6)
			<< " " << tb.getNTC(NTC_CH_L5L7)
			<< " " << tb.getNTC(NTC_CH_L6L8)
			<< " " << tb.getNTC(NTC_CH_L7L9)
			<< std::endl;

	      std::cout << run << " " << time << " " << dac << " " << ps.getCurrent()
			<< " " << tb.getNTC(NTC_CH_L1L3)
			<< " " << tb.getNTC(NTC_CH_L2L4)
			<< " " << tb.getNTC(NTC_CH_L3L5)
			<< " " << tb.getNTC(NTC_CH_L4L6)
			<< " " << tb.getNTC(NTC_CH_L5L7)
			<< " " << tb.getNTC(NTC_CH_L6L8)
			<< " " << tb.getNTC(NTC_CH_L7L9)
			<< std::endl;

	      std::this_thread::sleep_for(std::chrono::seconds(1));
	    }
	  run++;
        }
      ps.turnOff();
    }
  catch(ComIOException &e)
    {
      std::cout << e.what() << std::endl;
    }

  return 0;
}
