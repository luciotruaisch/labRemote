#include <iostream>
#include <cmath>

#ifdef FTDI
#include "I2CFTDICom.h"
#endif

#include "Logger.h"
#include "I2CCom.h"
#include "AMAC.h"
#include "Bk85xx.h"
#include "AgilentPs.h"
#include "Keithley24XX.h"

loglevel_e loglevel = logINFO;

int main(int argc, char* argv[])
{
#ifndef FTDI
  logger(logERROR) << "FTDI support not enabled.";
  return -1;
#else  
  //
  // Get settings from the command line
  if (argc < 3) {
    logger(logERROR) << "Not enough parameters!";
    logger(logERROR) << "Useage: " << argv[0] << " <BK85XX> <GPIB>";
    return -1;
  }

  std::string bkDev = argv[1];
  std::string gpibDev = argv[2];

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

  logger(logINFO) << " ... Keithley 2410:";
  Keithley24XX sm(gpibDev, 9);
  try
    {
      sm.init();
      sm.setSource(KeithleyMode::CURRENT, 1e-6, 1e-6);
      sm.setSense(KeithleyMode::VOLTAGE, 500, 500);
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  logger(logINFO) << " ... DC Load:";
  Bk85xx dc(bkDev);
  dc.setRemote();
  dc.setRemoteSense(false);
  dc.setModeCC();
  dc.setCurrent(0);
  dc.turnOn();

  //
  // Poweroff
  sm.turnOff();
  ps.turnOff();
  dc.turnOff();
#endif // FTDI
  return 0;
}
