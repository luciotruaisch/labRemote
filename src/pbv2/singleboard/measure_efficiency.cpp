#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

#include "SerialCom.h"
#include "Logger.h"
#include "Bk85xx.h"
#include "AgilentPs.h"

loglevel_e loglevel = logDEBUG3;

int main(int argc, char* argv[]) {

    if (argc < 4) {
        logger(logERROR) << "Not enough parameters!";
        logger(logERROR) << "Usegae: " << argv[0] << " <DC LOAD device> <PS device> <outfile>";
        return 1;
    }

    logger(logINFO) << "Initialising ... ";
    Bk85xx dc(argv[1]);
    AgilentPs ps(argv[2], 10);

    logger(logINFO) << "Setting up BK8500 .. ";
    dc.setRemote();
    dc.setModeCC();
    dc.setCurrent(300.0);
    dc.getCurrent();
    dc.setRemoteSense();

    logger(logINFO) << "Setting up Agilent PS .. ";
    ps.init();
    //ps.setCh(1);
    ps.setRange(20);
    ps.setVoltage(10.0);
    ps.setCurrent(2.00);

    sleep(1);

    std::ofstream file(argv[3]);

    double vin_min = 8;
    double vin_max = 12;
    double vin_step = 2;

    double iout_min = 100;
    double iout_max = 3500;
    double iout_step = 50;

    // Outer loop over VIN
    for (double vin = vin_min; vin<=vin_max; vin+=vin_step) {
        logger(logINFO) << "#########################";
        ps.setVoltage(vin);
        for (double iout = iout_min; iout<=iout_max; iout+=iout_step) {
            dc.setCurrent(iout);
            logger(logDEBUG) << "Turning on!";
            ps.turnOn();
            dc.turnOn();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            double iin = stof(ps.getCurrent());
            struct values val = dc.getValues();
            logger(logINFO) << vin << " " << iin << " " << (vin*iin) << " " << val.vol/1000.0 << " " << val.cur/1000.0 << " " << val.pow/1000.0;
            file << vin << " " << iin << " " << (vin*iin) << " " << val.vol/1000.0 << " " << val.cur/1000.0 << " " << val.pow/1000.0 << std::endl;
        }
        file << std::endl << std::endl;
        ps.turnOff();
        dc.turnOff();
    }
    
    logger(logINFO) << "Done!";

    return 0;
}
