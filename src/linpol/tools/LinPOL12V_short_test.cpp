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
AgilentPs ps(agiDev, 04);
  //logger(logINFO) << "Init TTITSX PS";
  //TTIMX180TPPs ps(agiDev, 10);
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

  Bk85xx dc(bkDev);
  try{
  dc.setRemote();
  dc.setRemoteSense(false);
  dc.setModeCC();
  dc.setCurrent(10);
  //dc.turnOn();
  }
catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }
  //Turn on and run tests
  ps.turnOn();
  dc.turnOn();

  //ADC's from comm patch
  EndeavourRawFTDI *FTDI_dev = new EndeavourRawFTDI(); 
  std::cout<<"3V3 Reading Test: "<<FTDI_dev->getADC()->read(0)<<std::endl;
  std::cout<<"1V4 Reading Test: "<<dc.getValues().vol<<std::endl;
  std::cout<<"Curr 1V4, TOT: "<<std::stod(ps.getCurrent())<<" : "<<dc.getValues().cur<<std::endl;
  

  //Power off
  // logger(logINFO) << "Power off";
  // ps.turnOff();
  // dc.turnOff();


  // return 0;

  // dc.setCurrent(50);


  std::string fileName = outDir + "/" + LinPOL12VTestTools::getTimeAsString(std::chrono::system_clock::now()) + "_LinPOL12V_char.json";
  logger(logINFO) << "Results stored in " << fileName;
  std::fstream outfile(fileName, std::ios::out);
  if(!outfile.is_open())
    {
      logger(logERROR) << "Unable to create results file " << fileName;
      return 2;
    }
  json testSum;
  testSum["program"] = argv[0];
  
  
  testSum["test"] = LinPOL12VTestTools::testVOut_1L(dynamic_cast<GenericPs*>(&ps), &dc, FTDI_dev, 0,60,10,50);
  
  outfile << std::setw(4) << testSum << std::endl;
    
  outfile.close();
    
  //Power off
  logger(logINFO) << "Power off";
  ps.turnOff();
  dc.turnOff();

  return 0;
}
