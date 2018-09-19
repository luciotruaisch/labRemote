#ifndef PBV3TESTTOOLS_H
#define PBV3TESTTOOLS_H

#include "AMACv2.h"

#include "Bk85xx.h"
#include "GenericPs.h"
#include "Keithley24XX.h"

#include <chrono>
#include <thread>
#include <json.hpp>
#include <iomanip>
#include <string>

#include "Logger.h"

using nlohmann::json;

namespace PBv3TestTools {
    // All helper functions expects all hardware to be on and initialised
    json testLvEnable(AMACv2 *amac, GenericPs *ps, Bk85xx *load);
    json testHvEnable(AMACv2 *amac, Keithley24XX *sm, unsigned frequency=0x3);
    json measureEfficiency(AMACv2 *amac, GenericPs *ps, Bk85xx *load, int step, int min, int max);
    json readStatus(AMACv2 *amac, GenericPs *ps, Bk85xx *load, Keithley24XX *sm);
    json runBER(AMACv2 *amac);
    json calibrateAMAC(AMACv2 *amac, double step);

    std::string getTimeAsString(std::chrono::system_clock::time_point t);
}

#endif
