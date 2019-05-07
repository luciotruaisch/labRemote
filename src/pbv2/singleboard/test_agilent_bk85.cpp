#include <iostream>
#include "SerialCom.h"
#include "Logger.h"
#include "Bk85xx.h"
#include "AgilentPs.h"

loglevel_e loglevel = logDEBUG3;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        logger(logERROR) << "Not enough parameters!";
        logger(logERROR) << "Usegae: " << argv[0] << " /dev/ttyS0 /dev/ttyS1";
        return 1;
    }

    logger(logINFO) << "Initialising ... ";
    Bk85xx dc(argv[1]);
    AgilentPs ps(argv[2], 10);

    logger(logINFO) << "Setting up BK8500 .. ";
    dc.setRemote();
    dc.setModeCC();
    dc.setCurrent(300.0);
    dc.getCurrent();

    logger(logINFO) << "Setting up Agilent PS .. ";
    ps.init();
    ps.setCh(1);
    ps.setRange(20);
    ps.setVoltage(10.0);
    ps.setCurrent(0.500);

    sleep(1);

    logger(logINFO) << "Here we go ...";
    dc.turnOn();
    ps.turnOn();

    while (1) {
        sleep(1);
        logger(logINFO) << "PS Current: " << ps.getCurrent();
        struct values val = dc.getValues();
        logger(logINFO) << "DC Voltage: " << val.vol;
        logger(logINFO) << "DC Current: " << val.cur;
        logger(logINFO) << "DC Power: " << val.pow;
        logger(logINFO) << "##############################";
    }


    return 0;
}
