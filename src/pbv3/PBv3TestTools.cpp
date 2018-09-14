#include "PBv3TestTools.h"

#include "EndeavourComException.h"

namespace PBv3TestTools {
    int testLvEnable(AMACv2 *amac, AgilentPs *ps, Bk85xx *load) {
        log(logINFO) << "Starting LV Enable test:";
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

}
