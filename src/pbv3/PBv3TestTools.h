#ifndef PBV3TESTTOOLS_H
#define PBV3TESTTOOLS_H

#include "AMACv2.h"

#include "Bk85xx.h"
#include "AgilentPs.h"

#include <chrono>
#include <thread>

#include "Logger.h"

namespace PBv3TestTools {
    // All helper functions expects all hardware to be on and initialised
    int testLvEnable(AMACv2 *amac, AgilentPs *ps, Bk85xx *load);
    int measureEfficiency(AMACv2 *amac, AgilentPs *ps, Bk85xx *load, int step, int min, int max);

}

#endif
