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
    logger(logINFO) << "Init TTITSX PS";
    // AgilentPs ps(agiDev, 10);
    //TTITSX1820PPs ps(agiDev, 11);
    TTIMX180TPPs ps(agiDev, 10);
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
    //Load turned on w/ nonzero current
    dc.setCurrent(0);

    // Init Keithley2410
    logger(logINFO) << "Init Keithley 2410";
    Keithley24XX sm(agiDev, 02);
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
	amac->initRegisters();
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

	// Output file
	std::string fileName = outDir + "/" + PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()) + "_pbv3-test.json";
	std::fstream outfile(fileName, std::ios::out);

	// Prepare the output structure
	json testSum;
	testSum["program"] = argv[0];
	testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()); 

	// Start testing
	//Run efficiency once to get baseline current (ie powercycle)

	uint32_t test=0;
	try { testSum["tests"][test++] = PBv3TestTools::runBER(amac.get());                                                              } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	try { testSum["tests"][test++] = PBv3TestTools::testLvEnable(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc);                    } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	try { testSum["tests"][test++] = PBv3TestTools::testHvEnable(amac.get(), &sm);                                                   } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	try { testSum["tests"][test++] = PBv3TestTools::measureHvSense(amac.get(), &sm);                                                 } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }

	try { testSum["tests"][test++] = PBv3TestTools::measureEfficiency(amac.get(), dynamic_cast<GenericPs*>(&ps), &dc, 100, 0, 3500); } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	try { testSum["tests"][test++] = PBv3TestTools::calibrateAMACslope (amac.get(),0.1, false);                                      } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }
	try { testSum["tests"][test++] = PBv3TestTools::calibrateAMACoffset(amac.get()    , false);                                      } catch(const EndeavourComException &e) { logger(logERROR) << e.what(); }

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
