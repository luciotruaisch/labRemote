#include "PBv3TestTools.h"

#include "EndeavourComException.h"

namespace PBv3TestTools {
    int testLvEnable(AMACv2 *amac, AgilentPs *ps, Bk85xx *load) {
        log(logINFO) << "## LV Enable test ##";
        // Init stuff
        load->setCurrent(0);
        load->turnOn();
        try {
            amac->wrField(&AMACv2::DCDCen, 0);
            amac->wrField(&AMACv2::DCDCenC, 0);
        } catch(EndeavourComException &e) {
            log(logERROR) << e.what();
            return -1;
        }
    
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        double lv_off = load->getValues().vol;//mV
        log(logINFO) << " --> Reading " << lv_off << "mV in off state.";

        log(logINFO) << " --> Trying to turn on DCDC ...";
        try {
            amac->wrField(&AMACv2::DCDCen, 1);
            amac->wrField(&AMACv2::DCDCenC, 1);
        } catch(EndeavourComException &e) {
            log(logERROR) << e.what();
            return -1;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        double lv_on = load->getValues().vol;//mV
        log(logINFO) << " --> Reading " << lv_on << "mV in off state.";

        if (!(lv_on > 1.4e3 && lv_on < 1.6e3 && lv_off < 0.1e3)) {
            log(logERROR) << " --> LV enable not working! " << lv_on << " " << lv_off;
            return -1;
        } else {
            log(logINFO) << " --> LV enable good!";
        }

        return 0;

    }

    // Range is iout in mA
    int measureEfficiency(AMACv2 *amac, AgilentPs *ps, Bk85xx *load, int step, int min, int max) {
        log(logINFO) << "## Measuring DCDC efficiency ##";
        load->setCurrent(0);
        load->turnOn();
        try {
            amac->wrField(&AMACv2::DCDCen, 0);
            amac->wrField(&AMACv2::DCDCenC, 0);
        } catch(EndeavourComException &e) {
            log(logERROR) << e.what();
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        double Iin_offset = std::stod(ps->getCurrent());
        log(logINFO) << " --> Get baseline current: " << Iin_offset << "A";
        
        log(logINFO) << " --> Turn on DCDC ...";
        try {
            amac->wrField(&AMACv2::DCDCen, 1);
            amac->wrField(&AMACv2::DCDCenC, 1);
        } catch(EndeavourComException &e) {
            log(logERROR) << e.what();
            return -1;
        }

        unsigned dwell_time = 5; //s

        log(logINFO) << " --> Starting measurement ...";
        std::cout << "Vin" << "\t" << "Iin" << "\t\t" << "Vout" << "\t" << "Iout" << "\t" << "Vdcdc"
            << "\t" << "VddLr" << "\t" << "DCDCin" << "\t" << "NTC" << "\t"
            << "Cur10V" << "\t" << "Cur1V" << "\t" << "PTAT" << "\t" << "Efficiency" << std::endl;
        
        // Set sub-channel
        amac->wrField(&AMACv2::Ch12Mux, 0); //a
        amac->wrField(&AMACv2::Ch13Mux, 0); //a
        
        // Loop over currents
        for (int iout=min;iout<=max;iout+=step) {
            log(logDEBUG) << " --> Setting " << iout << "mA load!";
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

}
