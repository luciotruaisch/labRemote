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
#include "PBv3TestTools.h"
#include "PBv3Monitor.h"
#include "Keithley24XX.h"

loglevel_e loglevel = logINFO;

#define PERIOD_LONG 600
#define PERIOD_MONITOR 1

int main(int argc, char* argv[]) {
    
    if (argc < 4) {
        logger(logERROR) << "Not enough arguments!";
        logger(logERROR) << "Usage: " << argv[0] << " <data folder> <BK85XX> <GPIB>";
        return -1;
    }

    std::string outDir = argv[1];
    std::string bkDev  = argv[2];
    std::string agiDev = argv[3];
    
    // Prog ID set during init (require power-up to be set)
    unsigned short int amacid=0x0;

    // Init Agilent
    logger(logINFO) << "Init Agilent PS";
    TTITSX1820PPs ps(agiDev, 11);
    try
    {
        ps.init();
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
    dc.turnOn();

    // Init Keithley2410
    logger(logINFO) << "Init Keithley 2410";
    Keithley24XX sm(agiDev, 23);
    try
    {
        sm.init();
        sm.setSource(KeithleyMode::CURRENT, 0.5e-6, 0.5e-6);
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

    // Init com
    logger(logINFO) << "Init AMAC";
    std::unique_ptr<AMACv2> amac;
    try {
        amac.reset(new AMACv2(amacid, std::unique_ptr<EndeavourRaw>(new EndeavourRawFTDI())));
        amac->init();
    } catch(EndeavourComException &e) {
        logger(logERROR) << e.what();
        return 1;
    }

    // 
    // Start running tests in a loop forever!
    //
    for(uint32_t i=0; i<10; i++)
      {
	//
	// Run the long tests

	// Output file
	std::string fileName = outDir + "/" + PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()) + "_pbv3-test.json";
	std::fstream outfile(fileName, std::ios::out);

	// Prepare the output structure
	json testSum;
	testSum["program"] = argv[0];
	testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 

	// Start testing
	uint32_t test=0;
	try { testSum["tests"][test++] = PBv3TestTools::testLvEnable(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc);                    } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	//try { testSum["tests"][test++] = PBv3TestTools::testHvEnable(amac.get(), &sm);                                                   } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	try { testSum["tests"][test++] = PBv3TestTools::measureEfficiency(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc, 100, 0, 3500); } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	try { testSum["tests"][test++] = PBv3TestTools::runBER(amac.get());                                                              } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	try { testSum["tests"][test++] = PBv3TestTools::calibrateAMAC(amac.get(), 0.1);                                                  } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }

	testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	outfile << std::setw(4) << testSum << std::endl;
	outfile.close();

	//
	// Monitor things for a while
	dc.setCurrent(100);
	dc.turnOn();

	amac->wrField(&AMACv2::DCDCen, 1);
	amac->wrField(&AMACv2::DCDCenC, 1);

	for(uint32_t iMon=0;iMon<PERIOD_LONG/PERIOD_MONITOR;iMon++)
	  {
	    std::string fileName = outDir + "/" + PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()) + "_pbv3-monitor.json";
	    std::fstream outfile(fileName, std::ios::out);

	    // Prepare the output structure
	    json testSum;
	    testSum["program"] = argv[0];
	    testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

	    // Start testing
	    try 
	      {
		testSum["tests"][0] = PBv3TestTools::readStatus(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc, &sm);
	      }
	    catch(const EndeavourComException &e)
	      { logger(logERROR) << e.what(); }

	    testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 
	    outfile << std::setw(4) << testSum << std::endl;
	    outfile.close();
	  }

	amac->wrField(&AMACv2::DCDCen, 0);
	amac->wrField(&AMACv2::DCDCenC, 0);

	dc.turnOff();
      }

    //
    // Power off
    ps.turnOff();
    dc.turnOff();
    sm.turnOff();

    return 0;
}
