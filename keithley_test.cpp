#include <iostream>

#include "Logger.h"
#include "Keithley24XX.h"

loglevel_e loglevel = logINFO;

int main(int argc, char*argv[]) {

    Keithley24XX meter(argv[1], 9);
    meter.init();

    meter.setSource(KeithleyMode::VOLTAGE, 1.0, 1.0);
    meter.setSense(KeithleyMode::CURRENT, 0.1, 0.1);

    meter.turnOn();

    for (unsigned i=0; i<10; i++) {
        log(logINFO) << meter.sense();
        sleep(1);
    }

    meter.turnOff();

    return 0;
}

