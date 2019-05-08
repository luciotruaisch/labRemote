#include <iostream>

#include "Logger.h"
#include "Keithley24XX.h"

loglevel_e loglevel = logINFO;

int main(int argc, char*argv[]) {

  Keithley24XX meter(argv[1], 23);
  meter.init();

  meter.setSource(KeithleyMode::CURRENT, 1e-3, 1e-3);
  meter.setSense(KeithleyMode::VOLTAGE, 10, 10);

  meter.turnOn();

  for (unsigned i=0; i<10; i++) {
    logger(logINFO) << meter.sense(KeithleyMode::VOLTAGE);
    sleep(1);
  }

  meter.turnOff();

  return 0;
}

