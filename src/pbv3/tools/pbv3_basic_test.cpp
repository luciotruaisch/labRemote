#include <unistd.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <json.hpp>
#include <fstream>

#include "Logger.h"

#include "EndeavourRawFTDI.h"
#include "EndeavourCom.h"
#include "EndeavourComException.h"
#include "AMACv2.h"

#include "Bk85xx.h"
#include "AgilentPs.h"
#include "TTITSX1820PPs.h"
#include "TTIMX180TPPs.h"
#include "PBv3TestTools.h"
#include "Keithley24XX.h"

loglevel_e loglevel = logINFO;


int main(int argc, char* argv[]) 
{    
  if (argc < 4) {
    logger(logERROR) << "Not enough arguments!";
    logger(logERROR) << "Usage: " << argv[0] << " <data folder> <BK85XX> <GPIB>";
    return -1;
  }
    
  json testSum;
  testSum["program"] = argv[0];
  testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
  std::string outDir = argv[1];
  std::string bkDev = argv[2];
  std::string agiDev = argv[3];

  // Output file
  std::string fileName = outDir + "/" + PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()) + "_pbv3-test.json";
  logger(logINFO) << "Results stored in " << fileName;
  std::fstream outfile(fileName, std::ios::out);

  // Prog ID set during init (require power-up to be set)
  unsigned short int amacid=0x0;

  // Init Agilent
  //logger(logINFO) << "Init Agilent PS";
  logger(logINFO) << "Init TTITSX PS";
  //    AgilentPs ps(agiDev, 10);
  TTIMX180TPPs ps(agiDev, 10);
  //TTITSX1820PPs ps(agiDev, 10);
  try
    {
      ps.init();
      ps.setCh(1);
      ps.setRange(20);
      ps.setVoltage(11.0);
      ps.setCurrent(2.00);
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }
    
  // Init DCDC
  logger(logINFO) << "Init BK DCDC Load";
  Bk85xx dc(bkDev);
  dc.setRemote();
  dc.setRemoteSense(true);
  dc.setModeCC();
  dc.setCurrent(0);

  // Init Keithley2410
  logger(logINFO) << "Init Keithley 2410";
  Keithley24XX sm(agiDev, 02);
  try
    {
      sm.turnOff();
      sm.init();

      sm.setSource(KeithleyMode::CURRENT, 1e-6, 1e-6);
      sm.setSense(KeithleyMode::VOLTAGE, 500, 500);
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }
    
  // Measure LV IV
  uint32_t test=0;
  //testSum["tests"][test++] = PBv3TestTools::measureLvIV(dynamic_cast<GenericPs*>(&ps));

  // Turn on power
  logger(logINFO) << "Turn on PS fully";
  ps.setVoltage(11.0);
  ps.setCurrent(2.00);
  ps.turnOn();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
  // Init com
  logger(logINFO) << "Init AMAC";
  std::unique_ptr<AMACv2> amac;
  try {
    amac.reset(new AMACv2(amacid, std::unique_ptr<EndeavourRaw>(new EndeavourRawFTDI())));
    amac->init();
    amac->initRegisters();
  } catch(EndeavourComException &e) {
    logger(logERROR) << "Unable to initialize AMACv2";
    logger(logERROR) << e.what();
    return 1;
  }
    
  // Start testing
  testSum["tests"][test++] = PBv3TestTools::runBER(amac.get());
  testSum["tests"][test++] = PBv3TestTools::readStatus(amac.get()  , dynamic_cast<GenericPs*>(&ps), &dc, &sm);
  testSum["tests"][test++] = PBv3TestTools::testLvEnable(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc);
  testSum["tests"][test++] = PBv3TestTools::testHvEnable(amac.get(), &sm);
  //testSum["tests"][test++] = PBv3TestTools::calibVinResponse(amac.get(), dynamic_cast<GenericPs*>(&ps));
  testSum["tests"][test++] = PBv3TestTools::measureHvSense(amac.get(), &sm);
  testSum["tests"][test++] = PBv3TestTools::measureEfficiency(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc, 100, 0, 3500);
  testSum["tests"][test++] = PBv3TestTools::calibrateAMACoffset(amac.get() ,      false);
  testSum["tests"][test++] = PBv3TestTools::calibrateAMACslope (amac.get() , 0.1, false);

  testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
  outfile << std::setw(4) << testSum << std::endl;

  outfile.close();

  logger(logINFO) << "Power off";
  ps.turnOff();
  dc.turnOff();
  sm.turnOff();

  return 0;
}
