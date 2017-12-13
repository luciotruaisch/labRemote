#include <iostream>
#include "SerialCom.h"
#include "Logger.h"
#include "Bk85xx.h"
#include "AgilentPs.h"

loglevel_e loglevel = logDEBUG3;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        log(logERROR) << "Not enough parameters!";
        log(logERROR) << "Usegae: " << argv[0] << " /dev/ttyS0 /dev/ttyS1";
        return 1;
    }

    log(logINFO) << "Initialising ... ";
    Bk85xx dc(argv[1]);
    AgilentPs ps(argv[2], 10);

    log(logINFO) << "Setting up BK8500 .. ";
    dc.setRemote();
    dc.setModeCC();
    dc.setCurrent(300.0);
    dc.getCurrent();

    log(logINFO) << "Setting up Agilent PS .. ";
    ps.init();
    ps.setCh(1);
    ps.setRange(20);
    ps.setVoltage(10.0);
    ps.setCurrent(0.500);

    sleep(1);

    log(logINFO) << "Here we go ...";
    dc.turnOn();
    ps.turnOn();

    while (1) {
        sleep(1);
        log(logINFO) << "PS Current: " << ps.getCurrent();
        struct values val = dc.getValues();
        log(logINFO) << "DC Voltage: " << val.vol;
        log(logINFO) << "DC Current: " << val.cur;
        log(logINFO) << "DC Power: " << val.pow;
        log(logINFO) << "##############################";
    }


    return 0;
}
