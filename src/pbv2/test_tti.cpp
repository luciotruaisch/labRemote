#include <iostream>
#include "SerialCom.h"
#include "Logger.h"
#include "TTITSX1820PPs.h"

loglevel_e loglevel = logDEBUG3;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        logger(logERROR) << "Not enough parameters!";
        logger(logERROR) << "Usegae: " << argv[0] << " /dev/ttyS0";
        return 1;
    }

    TTITSX1820PPs ps(argv[1], 11);

    logger(logINFO) << "Setting up TTI TSX1820P PS .. ";
    ps.init();
    //ps.setCh(1);
    //ps.setRange(20);
    logger(logINFO) << "Set the correct adddress";
    ps.setVoltage(10.0);
    ps.setCurrent(0.500);

    sleep(1);

    logger(logINFO) << "Here we go ...";
    ps.turnOn();

    while (1) {
        sleep(1);
        logger(logINFO) << "PS Current: (" << ps.getCurrent() << ")";
	logger(logINFO) << "PS Voltage: (" << ps.getVoltage() << ")";
	
    }


    return 0;
}
