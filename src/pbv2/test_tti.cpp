#include <iostream>
#include "SerialCom.h"
#include "Logger.h"
#include "TTITSX1820PPs.h"

loglevel_e loglevel = logDEBUG3;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        log(logERROR) << "Not enough parameters!";
        log(logERROR) << "Usegae: " << argv[0] << " /dev/ttyS0";
        return 1;
    }

    TTITSX1820PPs ps(argv[1], 11);

    log(logINFO) << "Setting up TTI TSX1820P PS .. ";
    ps.init();
    //ps.setCh(1);
    //ps.setRange(20);
    log(logINFO) << "Set the correct adddress";
    ps.setVoltage(10.0);
    ps.setCurrent(0.500);

    sleep(1);

    log(logINFO) << "Here we go ...";
    ps.turnOn();

    while (1) {
        sleep(1);
        log(logINFO) << "PS Current: " << ps.getCurrent();
	log(logINFO) << "PS Voltage: " << ps.getVoltage();
	
    }


    return 0;
}
