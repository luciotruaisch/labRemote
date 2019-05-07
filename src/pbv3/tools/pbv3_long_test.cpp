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

#define PERIOD_LONG 600
#define PERIOD_MONITOR 1
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

  // Prog ID set during init (require power-up to be set)
  unsigned short int amacid=0x0;

  // Init Agilent
  logger(logINFO) << "Init Agilent PS";
  //AgilentPs ps(agiDev, 10);
  //logger(logINFO) << "Init TTITSX PS";
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
    

    // Turn on power
    logger(logINFO) << "Turn on PS";
    ps.turnOn();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    json config;
    if(!configfile.empty())
      {
	std::ifstream fh_in(configfile);
	if(fh_in.is_open())
	  fh_in >> config;
      }
    
    // Init com
    logger(logINFO) << "Init AMAC";
    std::shared_ptr<AMACv2> amac;
    try {
      amac.reset(new AMACv2(amacid, std::unique_ptr<EndeavourRaw>(new EndeavourRawFTDI())));
      amac->init();	
      PBv3ConfigTools::configAMAC(amac, config, false);        
      amac->initRegisters();
      PBv3ConfigTools::saveConfigAMAC(amac, config);
    } catch(EndeavourComException &e) {
      logger(logERROR) << e.what();
      return 1;
    }

    // 
    // Start running tests in a loop forever!
    //
    //time in minutes when the monitor is written to json files
    double time_write_mon =1.0;

    while(true)
      {
	//
	// Run the long tests

	//Run all calibrations
	json calib_all;// = PBv3ConfigTools::calibrateAll(amac);
	// Prepare the output structure
	json testSum;
	testSum["program"] = argv[0];
	testSum["config"] = config;
	testSum["calib_all"] = calib_all;
	testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 

	// Start testing
	//Run efficiency once to get baseline current (ie powercycle)

	uint32_t test=0;
	
	try { testSum["tests"][test++] = PBv3TestTools::runBER(amac);  } 
	catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	catch(std::string e) {logger(logERROR) << e; testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	  outfile << std::setw(4) << testSum << std::endl;
	  outfile.close();  break;}
	

	try { testSum["tests"][test++] = PBv3TestTools::testLvEnable(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc);  } 
	catch(const EndeavourComException &e) { logger(logERROR) << e.what(); } 
	catch(std::string e) {logger(logERROR) << e; testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	  outfile << std::setw(4) << testSum << std::endl;
	  outfile.close(); break;}
	
	try { testSum["tests"][test++] = PBv3TestTools::testHvEnable(amac.get(), &sm); } 
	catch(const EndeavourComException &e) { logger(logERROR) << e.what(); } 
	catch(std::string e) {logger(logERROR) << e; testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	  outfile << std::setw(4) << testSum << std::endl;
	  outfile.close();  break;}
	
	try { testSum["tests"][test++] = PBv3TestTools::measureHvSense(amac.get(), &sm);  }
	catch(const EndeavourComException &e) { logger(logERROR) << e.what(); } 
	catch(std::string e) {logger(logERROR) << e;testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	  outfile << std::setw(4) << testSum << std::endl;
	  outfile.close();  break;}

	try { testSum["tests"][test++] = PBv3TestTools::measureEfficiency(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc, 100, 0, 3500); }
	catch(const EndeavourComException &e) { logger(logERROR) << e.what(); } 
	catch(std::string e) {logger(logERROR) << e; testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	  outfile << std::setw(4) << testSum << std::endl;
	  outfile.close();  break;}

	try { testSum["tests"][test++] = PBv3TestTools::calibrateAMACslope (amac, 0.01, false);}
	catch(const EndeavourComException &e) { logger(logERROR) << e.what(); } 
	catch(std::string e) {logger(logERROR) << e; testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	  outfile << std::setw(4) << testSum << std::endl;
	  outfile.close(); break;}
	
	try { testSum["tests"][test++] = PBv3TestTools::calibrateAMACoffset(amac,      false); } 
	catch(const EndeavourComException &e) { logger(logERROR) << e.what(); } 
	catch(std::string e) {logger(logERROR) << e;testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
		  outfile << std::setw(4) << testSum << std::endl;
		  outfile.close();  break;}
	// try{  testSum["tests"][test++] = PBv3TestTools::calibrateAMACcm    (amac, 10000);}
	// catch(const EndeavourComException &e) { logger(logERROR) << e.what(); } 
	// catch(std::string e) {logger(logERROR) << e; testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	//   outfile << std::setw(4) << testSum << std::endl;
	//   outfile.close(); break;}
	

	testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	outfile << std::setw(4) << testSum << std::endl;
	outfile.close();
	
	//Check if 30min has elapsed
	auto t_out = std::chrono::system_clock::now();
	auto t_in = std::chrono::system_clock::now();
	auto t_in2 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapse = t_in-t_out;
	std::chrono::duration<double> elapse2 = t_in2-t_in;

	int iter = 0;
	//
	// Monitor things for a while (30 min explicitly here)

	// Enable load
	dc.setCurrent(2000);
	dc.turnOn();

	amac->wrField(&AMACv2::DCDCen, 1);
	amac->wrField(&AMACv2::DCDCenC, 1);

	// Enable HV
	sm.setSource(KeithleyMode::CURRENT, 1e-3, 1e-3);
	sm.setSense(KeithleyMode::VOLTAGE, 500, 500);
	sm.turnOn();

	amac->wrField(&AMACv2::CntSetHV0en, 1);
	amac->wrField(&AMACv2::CntSetCHV0en, 1);

	//for(uint32_t iMon=0;iMon<PERIOD_LONG/PERIOD_MONITOR;iMon++)
	while(elapse.count()/60. < 30.)
	{
	    std::string fileName = outDir + "/" + PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()) + "_pbv3-monitor.json";
	    std::fstream outfile(fileName, std::ios::out);

	    // Prepare the output structure
	    json testSum;
	    testSum["calibrate"]; // =  PBv3ConfigTools::calibrateAll(amac);
	    testSum["program"] = argv[0];
	    testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

	    // Start testing ( 1 min, then the file is written)
	    while(elapse2.count()/60. < time_write_mon)
	      {
		try
		  {
		    testSum["tests"][iter] = PBv3TestTools::readStatus(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc, &sm);
		  }
		catch(const EndeavourComException &e)
		  { logger(logERROR) << e.what(); }
		catch(std::string e) {logger(logERROR) << e; 
		  testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
		  outfile << std::setw(4) << testSum << std::endl;
		  outfile.close(); 
		  break;}

		t_in2 = std::chrono::system_clock::now();
		elapse2 = t_in2-t_in;

		iter++;
	      }
	    std::cout<<elapse2.count()/60.<<std::endl;
	    
	    iter = 0;

	    testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	    outfile << std::setw(4) << testSum << std::endl;
	    outfile.close();
	    //updating elapsed time
	    t_in = std::chrono::system_clock::now();
	    t_in2 = std::chrono::system_clock::now();
	    elapse = t_in-t_out;
	    elapse2 = t_in2-t_in;
	}

	amac->wrField(&AMACv2::DCDCen, 0);
	amac->wrField(&AMACv2::DCDCenC, 0);
	dc.turnOff();

	amac->wrField(&AMACv2::CntSetHV0en, 0);
	amac->wrField(&AMACv2::CntSetCHV0en, 0);
	sm.turnOff();
	

      }

    //
    // Power off
    ps.turnOff();
    dc.turnOff();
    sm.turnOff();

    return 0;
}
