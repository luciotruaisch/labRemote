#include <iostream>
#include <fstream>

#include "Logger.h"
#include "AgilentPs.h"

using namespace std;

loglevel_e loglevel = logDEBUG4;

int main(int argc, char*argv[]){

        ofstream outputfile;
        outputfile.open("output.txt"); //create output file and open
        cout << "Output file is created and opened" << endl;
        outputfile << "Voltage [V]   Current [uA] \n" << endl;

        AgilentPs meter(argv[1], 4);
        meter.init();
	
	meter.setRange(8);

        meter.setVoltage(0.0); //(KeithleyMode::VOLTAGE or KeithleyMode::CURRENT, Compliance, Value)

        meter.turnOn();
	
        for(unsigned i=0.0; i<8.0; i++){
                meter.setVoltage(i); //do an IV from 0-9V.
                log(logINFO) << meter.getCurrent();
                outputfile << meter.getVoltage() << "   "  << meter.getCurrent() << "\n"; //writing voltage and current to output file        
		
                sleep(1); //1 second 
		}

        outputfile.close();
	cout << "Output file is now closed" << endl;
	
	unsigned rampdown_v=9.0;
	while(rampdown_v!=0.0){
		rampdown_v--;
		meter.setVoltage(rampdown_v); //ramp down the voltage from 9 to 0
		sleep(1); //1 second
	}


        meter.turnOff();

        return 0;
}
