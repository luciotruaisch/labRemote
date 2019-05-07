#include <unistd.h>
#include <string.h>
#include <getopt.h>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include <fstream>

#include "Logger.h"

#include "EndeavourRawFTDI.h"
#include "EndeavourCom.h"
#include "EndeavourComException.h"


#include "Bk85xx.h"
#include "AgilentPs.h"
#include "TTITSX1820PPs.h"
#include "TTIMX180TPPs.h"
#include "TTILD400P.h"

#include "LinPOL12VTestTools.h"

void usage(char* argv[])
{
  std::cerr << "Usage: " << argv[0] << " datadir BK85XX GPIB" << std::endl;
}


int main(int argc, char* argv[])
{
  if (argc < 4)
    {
      usage(argv);
      return 1;
    }



  if (argc-optind < 3)
    {
      std::cout << argc << " " << optind << " " << 1 << std::endl;
      std::cerr << "Required paths missing." << std::endl;
      std::cerr << std::endl;
      usage(argv);
      return 1;
    }

  std::string outDir = argv[optind++];
  std::string bkDev  = argv[optind++];
  std::string agiDev = argv[optind++];
  
  logger(logDEBUG) << "Settings";
  logger(logDEBUG) << " outDir: " << outDir;
  logger(logDEBUG) << " bkDev: "  << bkDev;
  logger(logDEBUG) << " agiDev: " << agiDev;
  




  //Initializing necessary devices
  // Init Agilent
  logger(logINFO) << "Init Agilent PS";
  //  AgilentPs ps(agiDev, 04);
  //logger(logINFO) << "Init TTITSX PS";
  TTIMX180TPPs ps(agiDev, 10);
  //TTITSX1820PPs ps(agiDev, 10);
  try
    {
      ps.init();
      ps.setCh(1);
      ps.setRange(20);
      ps.setVoltage(11.0);
      ps.setCurrent(0.1);
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  // Init DCDC
  logger(logINFO) << "Init BK DCDC Load";

  Bk85xx load1(bkDev);
  try{
    load1.setRemote();
    load1.setRemoteSense(false);
    load1.setModeCC();
    load1.setCurrent(10);
    //load1.turnOn();
  }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  TTILD400P load2(agiDev,25);
  try
    {
      load2.init();
      load2.setModeCC();
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  //Turn on and run tests
  ps.turnOn();
  load1.turnOn();
  load2.turnOn();



  while(true){
    std::string fileName = outDir + "/" + LinPOL12VTestTools::getTimeAsString(std::chrono::system_clock::now()) + "_LinPOL12V_test.json";
    logger(logINFO) << "Results stored in " << fileName;
    std::fstream outfile(fileName, std::ios::out);
    if(!outfile.is_open())
      {
	logger(logERROR) << "Unable to create results file " << fileName;
	return 2;
      }
    json testSum;
    testSum["program"] = argv[0];
    
    
    testSum["test"] = LinPOL12VTestTools::testVOut_2L(dynamic_cast<GenericPs*>(&ps), &load1, dynamic_cast<GenericLoad*>(&load2), 0, 60, 0, 20 , 10, 10, 10);
    
    outfile << std::setw(4) << testSum << std::endl;
    
    outfile.close();
    
  }//end while true
  
  //Power off
  logger(logINFO) << "Power off";
  ps.turnOff();
  load1.turnOff();
  load2.turnOff();

  return 0;
}
