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

loglevel_e loglevel = logINFO;


int main(int argc, char* argv[]) {
    
    if (argc < 3) {
        logger(logERROR) << "Not enough arguments!";
        logger(logERROR) << "Usage: " << argv[0] << " <BK85XX> <GPIB>";
        return -1;
    }
    
    std::string bkDev = argv[1];
    std::string agiDev = argv[2];

    // Init Agilent
    logger(logINFO) << "Init Agilent PS";
    TTITSX1820PPs ps(agiDev, 11);
    try
    {
        ps.init();
    }
    catch(std::string e)
    {
        logger(logERROR) << e;
        return 1;
    }
    
    // Init DCDC
    logger(logINFO) << "Init BK DCDC Load";
    Bk85xx dc(bkDev);

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

    //
    // Power off
    logger(logINFO) << "Turn everything off!";
    ps.turnOff();
    dc.turnOff();
    sm.turnOff();



   return 0;
}

