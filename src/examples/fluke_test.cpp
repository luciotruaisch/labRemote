#include <iostream>

#include "Logger.h"
#include "Fluke8842.h"

loglevel_e loglevel = logINFO;

int main(int argc, char*argv[]) {

  Fluke8842 meter(argv[1], 6);
  meter.init();

  meter.setSense(FlukeMode::VOLTAGEDC);

  for (unsigned i=0; i<10; i++) {
    logger(logINFO) << meter.sense();
    //std::cout<<meter.sense()<<std::endl;
    sleep(1);
  }

  return 0;
}

