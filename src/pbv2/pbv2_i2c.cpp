#include <iostream>
#include <fstream>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>

#ifdef FTDI
#include "FTDICom.h"
#endif

#include "Logger.h"
#include "MojoCom.h"
#include "I2CCom.h"
#include "AMAC.h"
#include "AgilentPs.h"
#include "Keithley24XX.h"

loglevel_e loglevel = logINFO;

int main(int argc, char* argv[]) {
  //
  // Get settings from the command line
  if (argc < 4) {
    logger(logERROR) << "Not enough parameters!";
    logger(logERROR) << "Usage: " << argv[0] << " TESTNAME <Mojo/FTDI> <GPIB>";
    return -1;
  }

  std::string TestName = argv[1];
  std::string mojoDev = argv[2];
  std::string gpibDev = argv[3];

#ifndef FTDI
  if(mojoDev=="FTDI")
    {
      logger(logERROR) << "FTDI support not enabled.";
      return -1;
    }
#endif

  //
  // Create log directory if it does not exist
  DIR* logdir=opendir("log");
  if(!logdir)
    {
      const int dir_err = mkdir("log", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      if (-1 == dir_err)
	{
	  std::cerr << "Error creating log directory!" << std::endl;
	  return 1;
	}
    }
  else
    closedir(logdir);

  //
  // Run tests
  logger(logINFO) << "Initialising ...";

  logger(logINFO) << " ... Agilent PS:";
  AgilentPs ps(gpibDev, 10);
  try
    {
      ps.init();
      ps.setRange(20);
      ps.setVoltage(11.0);
      ps.setCurrent(2.00);
      ps.turnOn();
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  std::shared_ptr<I2CCom> i2c;
#ifdef FTDI
  if(mojoDev=="FTDI")
    i2c.reset(new FTDICom());
  else
    i2c.reset(new MojoCom(mojoDev));
#else
  i2c.reset(new MojoCom(mojoDev));
#endif
  for(uint amacid=0;amacid<16;amacid++)
    {
      logger(logINFO) << " Testing AMAC ID" << amacid;
      AMAC amac(amacid, i2c);

      int error_ctr = 0, num_attempts_per_val=100;
      for(int j = 0; j < num_attempts_per_val; j++)
	{
	  unsigned val_to_write = rand()%256;
	  unsigned val_received = 0;
	  //Write to any RW register
	  if(amac.write(AMACreg::ILOCK_HV_THRESH_HI_L_CH0,val_to_write))
	    {
	      std::cout << "Write error" << std::endl;
	      error_ctr++;
	    }
	  else
	    {
	      if(amac.read(AMACreg::ILOCK_HV_THRESH_HI_L_CH0,val_received))
		{
		  std::cout << "Read error" << std::endl;
		  error_ctr++;
		}
	      else if(val_to_write != val_received)
		{
		  std::cout << "Data error: 0x" << std::hex << val_to_write << " != 0x" << val_received << std::dec << std::endl;
		  error_ctr++;
		}
	    }
	}
      double result = 1.0 - (double)error_ctr / num_attempts_per_val;
      std::cout << "reliability = " << result << std::endl;
      if(result>0) break;
    }

  //
  // Poweroff
  ps.turnOff();

  return 0;
}
