#include <unistd.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#include "Logger.h"

#include "EndeavourRawFTDI.h"
#include "EndeavourCom.h"
#include "EndeavourComException.h"
#include "AMACv2.h"

#include "Bk85xx.h"
#include "AgilentPs.h"

loglevel_e loglevel = logINFO;


int main(int argc, char* argv[]) {
    
    if (argc < 3) {
        log(logERROR) << "Not enough arguments!";
        log(logERROR) << "Usage: " << argv[0] << " <BK85XX> <GPIB>";
        return -1;
    }
    
    std::string bkDev = argv[1];
    std::string agiDev = argv[2];

    // Prog ID set during init (require power-up to be set)
    unsigned short int amacid=0x0;

    // Init Agilent
    log(logINFO) << "Init Agilent PS";
    AgilentPs ps(agiDev, 10);
    try
    {
        ps.init();
        ps.setRange(20);
        ps.setVoltage(11.0);
        ps.setCurrent(2.00);
    }
    catch(std::string e)
    {
        log(logERROR) << e;
        return 1;
    }
    
    // Init DCDC
    log(logINFO) << "Init BK DCDC Load";
    Bk85xx dc(bkDev);
    dc.setRemote();
    dc.setRemoteSense(false);
    dc.setModeCC();
    dc.setCurrent(0);
    dc.turnOn();

    // Turn on power
    log(logINFO) << "Turn on PS";
    ps.turnOn();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Init com
    log(logINFO) << "Init AMAC";
    std::unique_ptr<AMACv2> amac;
    try {
        amac.reset(new AMACv2(amacid, std::unique_ptr<EndeavourRaw>(new EndeavourRawFTDI())));
    } catch(std::string e) {
        log(logERROR) << e;
        return 1;
    }

    // Test DCDC enable
    double lv_off = dc.getValues().vol;//mV

    log(logINFO) << "Trying to turn on DCDC ...";
    try {
        amac->wrField(&AMACv2::DCDCen, 1);
        amac->wrField(&AMACv2::DCDCenC, 1);
    } catch(std::string e) {
        log(logERROR) << e;
        return 1;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    double lv_on = dc.getValues().vol;//mV

    if (!(lv_on > 1.4e3 && lv_on < 1.6e3 && lv_off < 0.1e3)) {
        log(logERROR) << " ++ LV enable not working! " << lv_on << " " << lv_off;
        return -1;
    } else {
        log(logINFO) << " ++ LV enable good!";
    }

}
