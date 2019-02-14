#include <unistd.h>
#include <string.h>
#include <getopt.h>

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
#include "Keithley24XX.h"

#include "PBv3TestTools.h"
#include "PBv3ConfigTools.h"

//------ SETTINGS
loglevel_e loglevel = logINFO;
std::string configfile = "config.json";
//---------------

void usage(char* argv[])
{
  std::cerr << "Usage: " << argv[0] << " [options] datadir BK85XX GPIB" << std::endl;
  std::cerr << "List of options:" << std::endl;
  std::cerr << " -c, --config      Config for initializing the AMAC. (default: " << configfile << ")" << std::endl;
  std::cerr << " --debug           Enable more verbose printout"  << std::endl;
  std::cerr << "" << std::endl;
  std::cerr << "" << std::endl;
}

int main(int argc, char* argv[]) 
{
  if (argc < 4)
    {
      usage(argv);
      return 1;
    }

  int c;
  const int DEBUG_OPTION = 1000;
  while (1)
    {
      int option_index = 0;
      static struct option long_options[] =
	{
	  {"config",   required_argument, 0,  'c' },
	  {"debug",    no_argument      , 0,  DEBUG_OPTION },
	  {0,          0                , 0,  0 }
	};

      c = getopt_long(argc, argv, "c:", long_options, &option_index);
      if (c == -1)
	break;

      switch (c)
	{
	case 'c':
	  configfile = optarg;
	  break;
	case DEBUG_OPTION:
	  loglevel = logDEBUG;
	  break;
	default:
	  std::cerr << "Invalid option supplied. Aborting." << std::endl;
	  std::cerr << std::endl;
	  usage(argv);
	}
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

  // Output file
  std::string fileName = outDir + "/" + PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()) + "_pbv3-test.json";
  logger(logINFO) << "Results stored in " << fileName;
  std::fstream outfile(fileName, std::ios::out);
  if(!outfile.is_open())
    {
      logger(logERROR) << "Unable to create results file " << fileName;
      return 2;
    }

  // Prog ID set during init (require power-up to be set)
  unsigned short int amacid=0x0;

  // Init Agilent
  logger(logINFO) << "Init Agilent PS";
  AgilentPs ps(agiDev, 10);
  //logger(logINFO) << "Init TTITSX PS";
  //TTIMX180TPPs ps(agiDev, 10);
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
  dc.setRemoteSense(false);
  dc.setModeCC();
  dc.setCurrent(0);

  // Init Keithley2410
  logger(logINFO) << "Init Keithley 2410";
  Keithley24XX sm(agiDev, 05);
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

  json config;
  if(!configfile.empty())
    {
      std::ifstream fh_in(configfile);
      if(fh_in.is_open())
	fh_in >> config;
    }


  logger(logINFO) << "Init AMAC";
  std::shared_ptr<AMACv2> amac;
  try {
    amac.reset(new AMACv2(amacid, std::unique_ptr<EndeavourRaw>(new EndeavourRawFTDI())));
    amac->init();
    PBv3ConfigTools::configAMAC(amac, config, false);
    amac->initRegisters();
    PBv3ConfigTools::saveConfigAMAC(amac, config);
  } catch(EndeavourComException &e) {
    logger(logERROR) << "Unable to initialize AMACv2";
    logger(logERROR) << e.what();
    return 1;
  }

  //
  // Start testing
  json testSum;
  testSum["program"] = argv[0];
  testSum["config"] = config;
  testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 

  testSum["tests"][test++] = PBv3TestTools::runBER(amac);
  testSum["tests"][test++] = PBv3TestTools::readStatus(amac.get()  , dynamic_cast<GenericPs*>(&ps), &dc, &sm);
  testSum["tests"][test++] = PBv3TestTools::testLvEnable(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc);
  testSum["tests"][test++] = PBv3TestTools::testHvEnable(amac.get(), &sm);
  //testSum["tests"][test++] = PBv3TestTools::calibVinResponse(amac.get(), dynamic_cast<GenericPs*>(&ps));
  testSum["tests"][test++] = PBv3TestTools::measureHvSense(amac.get(), &sm);
  testSum["tests"][test++] = PBv3TestTools::measureEfficiency(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc, 100, 0, 3500);
  testSum["tests"][test++] = PBv3TestTools::calibrateAMACoffset(amac,       false);
  testSum["tests"][test++] = PBv3TestTools::calibrateAMACslope (amac, 0.01, false);
  testSum["tests"][test++] = PBv3TestTools::calibrateAMACcm    (amac, 10000);

  testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
  outfile << std::setw(4) << testSum << std::endl;

  outfile.close();

  logger(logINFO) << "Power off";
  ps.turnOff();
  dc.turnOff();
  sm.turnOff();

  return 0;
}
