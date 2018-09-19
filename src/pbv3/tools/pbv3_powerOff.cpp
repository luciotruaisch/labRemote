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

    logger(logINFO) << "Turn everything off!";
    ps.turnOff();
    dc.turnOff();

   return 0;
}

