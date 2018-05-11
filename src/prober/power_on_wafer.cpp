#include <iostream>
#include <fstream>

#include "Logger.h"
#include "AgilentPs.h"

using namespace std;

loglevel_e loglevel = logDEBUG4;

int main(int argc, char*argv[]){

        AgilentPs meter(argv[1], 4); // ( , GPIB address)
        meter.init();
	
	//meter.setRange(8); //if power supply has multiple ranges, write maximum value of desired range here

        meter.setVoltage(1.85); //1.85V for Linear Regulator

        meter.turnOn();

	log(logINFO) << meter.getCurrent();


        return 0;
}
