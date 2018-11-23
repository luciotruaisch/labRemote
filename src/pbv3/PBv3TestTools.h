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

namespace PBv3TestTools
{
  // All helper functions expects all hardware to be on and initialised

  // LV tests
  json testLvEnable(AMACv2 *amac, GenericPs *ps, Bk85xx *load);
  json measureEfficiency(AMACv2 *amac, GenericPs *ps, Bk85xx *load, int step, int min, int max , double VinSet=11.0);
  json calibVinResponse(AMACv2 *amac, GenericPs *ps);
  json measureLvIV(GenericPs *ps);

  //
  // HV tests
  json testHvEnable(AMACv2 *amac, Keithley24XX *sm, unsigned frequency=0x3);
  json measureHvSense(AMACv2 *amac, Keithley24XX *sm);

  //
  // AMAC tests
  json runBER(AMACv2 *amac);
  json readStatus(AMACv2 *amac, GenericPs *ps, Bk85xx *load, Keithley24XX *sm);
  json calibrateAMACslope (AMACv2 *amac, double step, bool scanSettings=true);
  json calibrateAMACoffset(AMACv2 *amac,              bool scanSettings=true);
  json testOF(AMACv2 *amac, GenericPs *ps);

  std::string getTimeAsString(std::chrono::system_clock::time_point t);
}

#endif
