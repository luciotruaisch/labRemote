#include <iostream>
#include <fstream>

#include "Logger.h"
#include "AgilentPs.h"

loglevel_e loglevel = logDEBUG4;

int main(int argc, char*argv[]){
	
	std::ofstream outputfile;
	outputfile.open("output.txt"); //Create output file and open. Rewrite if there is already a file.
	std::cout << "Output file is created and opened" << std::endl;
	outputfile << "Voltage [V]	Current[A] \n" << std::endl;
        AgilentPs meter(argv[1], 4); // ( , GPIB address)
        meter.init();
	
	//meter.setRange(8); //if power supply has multiple ranges, write maximum value of desired range here
	
	//meter.setVoltageProtect(2);
	//meter.setCurrentProtect(1.00);
        meter.setVoltage(1.85); //1.85V for Linear Regulator

        meter.turnOn();

	log(logINFO) << meter.getCurrent();

	outputfile << meter.getVoltage() << "	" << meter.getCurrent() << "\n"; //writing voltage and current to output file.
	
	outputfile.close();
	std::cout << "Output file is now closed." << std::endl;

        return 0;
}
