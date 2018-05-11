#include <iostream>
#include <fstream>

#include "Logger.h"
#include "Keithley2000.h"

using namespace std;

loglevel_e loglevel = logDEBUG4;

int main(int argc, char*argv[]) {	//// ./bin/keithley2000_test /dev/ttyUSB0 4 (argv[0] argv[1] argv[2]->channel you want to read out).

	Keithley2000 meter(argv[1], 1);
	meter.init();
	//meter.setSense(KeithleyMode::VOLTAGE); //for front panel
	meter.readChannel(argv[2]);
	return 0;
}

