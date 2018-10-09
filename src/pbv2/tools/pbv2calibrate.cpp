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
      std::cout << "Parameters needed: <filename> <PBNumber>" << std::endl;
      return -1;
    }

  std::string filename = argv[1];
  int pb = strtol(argv[2], NULL, 10);

  try
    {
      PBv2TB tb;

      std::fstream logfile(filename, std::fstream::out);
      logfile << "DAC" << " " << "Iout" << std::endl;

      //initiate power supply 
      SorensenPs ps("/dev/ttyUSB0", 11);

      ps.init();
      ps.setVoltage(1.5);
      ps.setCurrent(5);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      ps.turnOn();
      std::cout << "Power supply turned on, voltage set to " << ps.getVoltage() <<
	" current set to " << ps.getCurrent();

      for(uint32_t i=0; i<0xFF; i++)
        {
	  tb.setLoadCounts(pb, i);
	  std::this_thread::sleep_for(std::chrono::milliseconds(500));

	  logfile   << i << " " << ps.getCurrent() << std::endl;
	  std::cout << i << " " << ps.getCurrent() << std::endl;
        }

      ps.turnOff();
    }

  catch(ComIOException &e)
    {
      std::cout << e.what() << std::endl;
    }

  return 0;
}
