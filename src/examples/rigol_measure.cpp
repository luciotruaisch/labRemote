#include <iostream>

#include "Logger.h"
#include "RigolDP832.h"

loglevel_e loglevel = logINFO;

int main(int argc, char*argv[]) {

//Run ./bin/rigol_measure /dev/usbtmc0 

    RigolDP832 testsource(argv[1]);
    //testsource.init();

    //testsource.setVoltageCurrent(1, 1.50, 2.0);
    //testsource.turnOn(1);
    //testsource.setVoltageCurrent(2, 1.50, 2.0);
    //testsource.turnOn(2);

    //sleep(1);

    int i=0;
	while(i<1) {
	  std::cout<<testsource.getVoltage(1)<<testsource.getCurrent(1)<<testsource.getVoltage(2)<<testsource.getCurrent(2)<<std::endl;
	  //std::cout<<i<<std::endl;
	  //i++;
	}

    //testsource.turnOff(1);
    //testsource.turnOff(2);

    return 0;
}
