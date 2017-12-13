#include <iostream>
#include <cmath>

#include "Logger.h"
#include "MojoCom.h"
#include "I2CCom.h"
#include "AMAC.h"
#include "Bk85xx.h"
#include "AgilentPs.h"
#include "Keithley24XX.h"

loglevel_e loglevel = logINFO;

int main(int argc, char* argv[]) {

    if (argc < 4) {
        log(logERROR) << "Not enough parameters!";
        log(logERROR) << "Useage: " << argv[0] << " <Mojo> <BK85XX> <GPIB>";
        return -1;
    }

    std::string mojoDev = argv[1];
    std::string bkDev = argv[2];
    std::string gpibDev = argv[3];

    log(logINFO) << "Initialising ...";
    
    log(logINFO) << " ... Agilent PS:";
    AgilentPs ps(gpibDev, 10);
    ps.init();
    ps.setRange(20);
    ps.setVoltage(11.0);
    ps.setCurrent(2.00);
    ps.turnOn();

    log(logINFO) << " ... Keithley 2410:";
    Keithley24XX sm(gpibDev, 9);
    sm.init();
    sm.setSource(KeithleyMode::CURRENT, 1e-6, 1e-6);
    sm.setSense(KeithleyMode::VOLTAGE, 500, 500);

    log(logINFO) << " ... DC Load:";
    Bk85xx dc(bkDev);
    dc.setRemote();
    dc.setRemoteSense();
    dc.setModeCC();
    dc.setCurrent(0);
    dc.turnOn();
  
    log(logINFO) << " ... AMAC:";
    MojoCom mojo(mojoDev);
    AMAC amac(0, dynamic_cast<I2CCom*>(&mojo));
    

    sm.turnOff();
    ps.turnOff();
    dc.turnOff();
    
    /*
    log(logINFO) << "  ++ADC Values";
    unsigned val = 0;
    amac.read(AMACreg::VALUE_RIGHT_CH0, val);
    log(logINFO) << "[VIN] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH1, val);
    log(logINFO) << "[IOUT] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH2, val);
    log(logINFO) << "[NTC_Temp] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH3, val);
    log(logINFO) << "[3R] : \t\t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH4, val);
    log(logINFO) << "[VDD_H/4] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH5, val);
    log(logINFO) << "[OTA] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH6, val);
    log(logINFO) << "[ICHANR] : \t" << val ;
    
    amac.read(AMACreg::VALUE_LEFT_CH0, val);
    log(logINFO) << "[0L] : \t\t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH1, val);
    log(logINFO) << "[DVDD/2] : \t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH2, val);
    log(logINFO) << "[BGO] : \t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH3, val);
    log(logINFO) << "[3L] : \t\t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH4, val);
    log(logINFO) << "[TEMP] : \t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH5, val);
    log(logINFO) << "[OTA] : \t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH6, val);
    log(logINFO) << "[ICHANL] : \t" << val ;

    */

    return 0;
}
