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

    std::string getTimeAsString(std::chrono::system_clock::time_point t) {
        auto as_time_t = std::chrono::system_clock::to_time_t(t);
        struct tm tm;
        char some_buffer[128];
        if (::gmtime_r(&as_time_t, &tm))
            if (std::strftime(some_buffer, sizeof(some_buffer), "%A-%d/%m/%Y", &tm))
                return std::string(some_buffer);
        throw std::runtime_error("Failed to get current date as string");
    }
}

#endif
