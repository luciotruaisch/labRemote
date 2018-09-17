#include "PBv3TestTools.h"

#include "EndeavourComException.h"

namespace PBv3TestTools {
    json testLvEnable(AMACv2 *amac, AgilentPs *ps, Bk85xx *load) {
        json testSum;
        testSum["name"] = "lv_enable";
        testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        testSum["sucess"] = false;
        logger(logINFO) << "## LV Enable test ##";
        // Init stuff
        load->setCurrent(0);
        load->turnOn();
        try {
            amac->wrField(&AMACv2::DCDCen, 0);
            amac->wrField(&AMACv2::DCDCenC, 0);
        } catch(EndeavourComException &e) {
            logger(logERROR) << e.what();
            return testSum;
        }
    
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        double lv_off = load->getValues().vol;//mV
        logger(logINFO) << " --> Reading " << lv_off << "mV in off state.";
        testSum["lvOff"]["V"] = lv_off;
        logger(logINFO) << " --> Trying to turn on DCDC ...";
        try {
            amac->wrField(&AMACv2::DCDCen, 1);
            amac->wrField(&AMACv2::DCDCenC, 1);
        } catch(EndeavourComException &e) {
            logger(logERROR) << e.what();
            return testSum;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        double lv_on = load->getValues().vol;//mV
        logger(logINFO) << " --> Reading " << lv_on << "mV in off state.";
        testSum["lvOn"]["V"] = lv_off;

        if (!(lv_on > 1.4e3 && lv_on < 1.6e3 && lv_off < 0.1e3)) {
            logger(logERROR) << " --> LV enable not working! " << lv_on << " " << lv_off;
            return testSum;
        } else {
            logger(logINFO) << " --> LV enable good!";
        }
        testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        testSum["sucess"] = true;
        return testSum;

    }

    // Range is iout in mA
    json measureEfficiency(AMACv2 *amac, AgilentPs *ps, Bk85xx *load, int step, int min, int max) {
        logger(logINFO) << "## Measuring DCDC efficiency ##";
        load->setCurrent(0);
        load->turnOn();
        try {
            amac->wrField(&AMACv2::DCDCen, 0);
            amac->wrField(&AMACv2::DCDCenC, 0);
        } catch(EndeavourComException &e) {
            logger(logERROR) << e.what();
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        double Iin_offset = std::stod(ps->getCurrent());
        logger(logINFO) << " --> Get baseline current: " << Iin_offset << "A";
        
        logger(logINFO) << " --> Turn on DCDC ...";
        try {
            amac->wrField(&AMACv2::DCDCen, 1);
            amac->wrField(&AMACv2::DCDCenC, 1);
        } catch(EndeavourComException &e) {
            logger(logERROR) << e.what();
            return -1;
        }

        unsigned dwell_time = 5; //s

        logger(logINFO) << " --> Starting measurement ...";
        std::cout << "Vin" << "\t" << "Iin" << "\t\t" << "Vout" << "\t" << "Iout" << "\t" << "Vdcdc"
            << "\t" << "VddLr" << "\t" << "DCDCin" << "\t" << "NTC" << "\t"
            << "Cur10V" << "\t" << "Cur1V" << "\t" << "PTAT" << "\t" << "Efficiency" << std::endl;
        
        // Set sub-channel
        amac->wrField(&AMACv2::Ch12Mux, 0); //a
        amac->wrField(&AMACv2::Ch13Mux, 0); //a
        
        // Loop over currents
        for (int iout=min;iout<=max;iout+=step) {
            logger(logDEBUG) << " --> Setting " << iout << "mA load!";
            // Set Current
            load->setCurrent(iout);
            // Wait for temp and everything to settle
            std::this_thread::sleep_for(std::chrono::seconds(dwell_time));
            // Read AMAC values
            int Vdcdc = amac->rdField(&AMACv2::Ch0Value);
            int VddLr = amac->rdField(&AMACv2::Ch1Value);
            int DCDCin = amac->rdField(&AMACv2::Ch2Value);
            int NTC = amac->rdField(&AMACv2::Ch9Value);
            int Cur10V = amac->rdField(&AMACv2::Ch12Value);
            int Cur1V = amac->rdField(&AMACv2::Ch13Value);
            int PTAT = amac->rdField(&AMACv2::Ch15Value);

            double Vin = std::stod(ps->getVoltage());
            double Iin = std::stod(ps->getCurrent());

            double Vout = load->getValues().vol;

            std::cout << Vin << "\t" << Iin << "\t" << Vout << "\t" << iout << "\t" << Vdcdc
                << "\t" << VddLr << "\t" << DCDCin << "\t" << NTC << "\t"
                << Cur10V << "\t" << Cur1V << "\t" << PTAT << "\t" <<  (1.5*iout*1e-3)/(Vin*(Iin-Iin_offset)) << std::endl;

        }
        
        return 0;

    }

    json testHvEnable(AMACv2 *amac, Keithley24XX *sm, unsigned frequency) {
        logger(logINFO) << "## Testing HV Enable circuit ##";
        logger(logINFO) << " --> Frequency: " << frequency << " (0=high, 1=25kHz, 2=50kHz, 3=100kHz)";
        logger(logINFO) << " --> Turning everything off.";

        // Turn off HV enable from AMAC
        // TODO test 
        amac->wrField(&AMACv2::CntSetHV0frq, frequency);
        amac->wrField(&AMACv2::CntSetCHV0frq, frequency);
        amac->wrField(&AMACv2::CntSetHV0en, 0);
        amac->wrField(&AMACv2::CntSetCHV0en, 0);

        // Configure sourcemeter
        sm->turnOff();
        sm->setSource(KeithleyMode::CURRENT, 1e-6, 1e-6);
        sm->setSense(KeithleyMode::VOLTAGE, 500, 500);

        // See what we see
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        sm->turnOn();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        double hv_v_off = std::stod(sm->sense(KeithleyMode::VOLTAGE));
        double hv_i_off = std::stod(sm->sense(KeithleyMode::CURRENT));
        logger(logINFO) << " Seeing " << hv_v_off << "V and " << hv_i_off << "A in off state.";

        // Turn on HV
        logger(logINFO) << " --> Turn on HV enable.";
        amac->wrField(&AMACv2::CntSetHV0en, 1);
        amac->wrField(&AMACv2::CntSetCHV0en, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        double hv_v_on = std::stod(sm->sense(KeithleyMode::VOLTAGE));
        double hv_i_on = std::stod(sm->sense(KeithleyMode::CURRENT));
        logger(logINFO) << " Seeing " << hv_v_off << "V and " << hv_i_off << "A in on state.";

        return 0;
    }

    int readStatus(AMACv2 *amac, AgilentPs *ps, Bk85xx *load, Keithley24XX *sm) {

        return 0;
    }

}
