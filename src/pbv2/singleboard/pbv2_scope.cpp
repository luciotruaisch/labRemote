#include <iostream>
#include <fstream>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>

#ifdef FTDI
#include "I2CFTDICom.h"
#endif

#include "Logger.h"
#include "I2CCom.h"
#include "AMAC.h"
#include "AgilentPs.h"
#include "Keithley24XX.h"

#ifdef SCOPE
#include "PS6000.h"
#endif

loglevel_e loglevel = logINFO;

int main(int argc, char* argv[])
{
#ifndef FTDI
  logger(logERROR) << "FTDI support not enabled.";
  return -1;
#else
#ifndef SCOPE
  logger(logERROR) << "Missing libScope";
#else // SCOPE
  //
  // Get settings from the command line
  if (argc < 3) {
    logger(logERROR) << "Not enough parameters!";
    logger(logERROR) << "Usage: " << argv[0] << " TESTNAME <GPIB>";
    return -1;
  }

  std::string TestName = argv[1];
  std::string gpibDev = argv[2];

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
  std::string logpath;
  std::fstream logfile;

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
  catch(std::string &e)
    {
      logger(logERROR) << e;
      return 1;
    }

  logger(logINFO) << " ... AMAC:";
  std::shared_ptr<I2CCom> i2c=std::make_shared<I2CFTDICom>(0x0);
  AMAC amac(i2c);

  logger(logINFO) << "  ++Init";
  amac.init();

  logger(logINFO) << "  ++Disable LV";
  amac.write(AMACreg::LV_ENABLE, 0x0);
  logger(logINFO) << "  ++Disable HV";
  amac.write(AMACreg::HV_ENABLE, 0x0);


  logger(logINFO) << "  ++ Const ADC values:";
  unsigned ota_l, ota_r, bgo, dvdd2;
  amac.read(AMACreg::VALUE_LEFT_CH5, ota_l);
  std::cout << "OTA LEFT : \t" << ota_l << std::endl;
  amac.read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  std::cout << "OTA_RIGHT : \t" << ota_r << std::endl;
  amac.read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  std::cout << "DVDD/2 : \t" << dvdd2 << std::endl;
  amac.read(AMACreg::VALUE_LEFT_CH2, bgo);
  std::cout << "BGO : \t" << bgo << std::endl;

  // Configure pico scope
  PicoScope *pico=new PS6000();
  pico->open();

  pico->setEnable(1,false);
  pico->setEnable(2,false);
  pico->setEnable(3,false);
  pico->setPeriod(1e-8);
  pico->setRange(0, 200);
  pico->configChannels();

  float period=pico->getPeriod();

  std::vector<std::vector<float>> data(1,std::vector<float>(PS6000_BUFFER_SIZE));

  // Run test with LV on
  logpath = "log/" + TestName + "_CoilLVON.log";
  logfile.open(logpath, std::fstream::out);

  amac.write(AMACreg::LV_ENABLE, 0x1);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  pico->run(data);

  logfile << "time coil" << std::endl;

  for(unsigned int row=0; row<data[0].size(); row++)
    logfile << row*period << " " << data[0][row] << std::endl;

  logfile.close();

  // Run test with LV off
  logpath = "log/" + TestName + "_CoilLVOFF.log";
  logfile.open(logpath, std::fstream::out);

  amac.write(AMACreg::LV_ENABLE, 0x0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  pico->run(data);

  logfile << "time coil" << std::endl;

  for(unsigned int row=0; row<data[0].size(); row++)
    logfile << row*period << " " << data[0][row] << std::endl;

  logfile.close();

  //
  // Poweroff
  pico->close();
  ps.turnOff();

#endif // SCOPE
#endif // FTDI
  return 0;
}
