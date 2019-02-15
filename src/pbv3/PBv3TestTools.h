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
  //
  // Helper functions for dealing with the result format
  
  /*
   * \brief Format the common information for the json test result.
   *
   * Adds the following fields:
   *  - component: From config json, DUMMY if not defined
   *  - institution: "LBL"
   *  - runNumber: from argument
   *  - properties/CONFIG: pointer to the configuration runNumber, from the config json
   *
   * \param config json object with the configuration used during the run
   * \param runNumber the run number to assign to this test
   *
   * \return The json object that can directly be merged with any test results
   */
  json testRunMetaData(json& config, const std::string& runNumber="0-0");

  //
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
  json runBER(std::shared_ptr<AMACv2> amac, uint32_t trails=1000);
  json readStatus(AMACv2 *amac, GenericPs *ps, Bk85xx *load, Keithley24XX *sm);
  json calibrateAMACslope (std::shared_ptr<AMACv2> amac, double step, bool scanSettings=true);
  json calibrateAMACoffset(std::shared_ptr<AMACv2> amac,              bool scanSettings=true);
  json calibrateAMACcm    (std::shared_ptr<AMACv2> amac, uint32_t tests=1000);
  json testOF(AMACv2 *amac, GenericPs *ps);

  std::string getTimeAsString(std::chrono::system_clock::time_point t);
}

#endif
