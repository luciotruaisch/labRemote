#include <iostream>

#include "Logger.h"
#include "SerialCom.h"
#include "MojoCom.h"
#include "I2CCom.h"
#include "AMAC.h"
#include "Bk85xx.h"
#include "AgilentPs.h"
loglevel_e loglevel = logINFO;

int main(int argc, char* argv[]) {

    if (argc < 4) {
        log(logERROR) << "Not enough parameters!";
        log(logERROR) << "Useage: " << argv[0] << " <Mojo> <BK85XX> <Agilent>";
        return -1;
    }

    std::string mojoDev = argv[1];
    std::string bkDev = argv[2];
    std::string agilentDev = argv[3];

    log(logINFO) << "Initialising ...";
    
    log(logINFO) << " ... Agilent PS:";
    AgilentPs ps(agilentDev, 10);
    ps.init();
    ps.setRange(20);
    ps.setVoltage(11.0);
    ps.setCurrent(2.00);
    ps.turnOn();

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
    
    log(logINFO) << "  ++Init";
    amac.write(AMACreg::BANDGAP_CONTROL, 10); //1.2V LDO output
    amac.write(AMACreg::RT_CH3_GAIN_SEL, 0); // no attenuation
    amac.write(AMACreg::LT_CH3_GAIN_SEL, 0); // no attentuation
    amac.write(AMACreg::RT_CH0_SEL, 1); //a
    amac.write(AMACreg::LT_CH0_SEL, 1); //a 
    amac.write(AMACreg::LEFT_RAMP_GAIN, 3); // best range
    amac.write(AMACreg::RIGHT_RAMP_GAIN, 3); // best range
    amac.write(AMACreg::OPAMP_GAIN_RIGHT, 0); // highest gain
    amac.write(AMACreg::OPAMP_GAIN_LEFT, 0); // highest gain
    amac.write(AMACreg::HV_FREQ, 0x1);
   
    log(logINFO) << "  ++Enable LV";
    amac.write(AMACreg::LV_ENABLE, 0x1);

    log(logINFO) << "  ++Enable HV";
    amac.write(AMACreg::HV_ENABLE, 0x1);
#if 0
    log(logINFO) << "Measuring Current Sense Amp ...";
    double iout_min = 100;
    double iout_max = 3500;
    double iout_step = 50;

    for (double iout = iout_min; iout <= iout_max; iout+=iout_step) {
        dc.setCurrent(iout);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        unsigned val = 0;
        amac.read(AMACreg::VALUE_RIGHT_CH2, val);
        log(logINFO) << iout << "mA =\t" << val;
    }

    dc.turnOff();
#endif
    log(logINFO) << "Measuring VIN measurement ...";
    amac.write(AMACreg::LV_ENABLE, 0x0);

    double vin_min = 6.0;
    double vin_max = 12.0;
    double vin_step = 0.1;

    for (double vin=vin_min; vin<=vin_max; vin+=vin_step) {
        ps.setVoltage(vin);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        unsigned val = 0;
        amac.read(AMACreg::VALUE_RIGHT_CH0, val);
        log(logINFO) << vin << "V = \t" << val;
    }



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

    for (unsigned int i= 1; i<16; i<<=1) {
        amac.write(AMACreg::OPAMP_GAIN_LEFT, i); 
        amac.read(AMACreg::VALUE_LEFT_CH6, val);
        log(logINFO) << "[" << i << "] [ICHANL] : \t" << val;
    }
    */

    return 0;
}
