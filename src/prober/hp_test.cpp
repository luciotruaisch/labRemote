#include <iostream>
#include <fstream>

#include "Logger.h"
#include "HP3478A.h"

using namespace std;

loglevel_e loglevel = logDEBUG4;

int main(int argc, char*argv[]) {	//// ./bin/keithley2000_test /dev/ttyUSB0 4 (argv[0] argv[1] argv[2]->channel you want to read out).

	HP3478A meter(argv[1], 4);
	meter.setSense(HPMode::DC_VOLTAGE);
	return 0;
}

