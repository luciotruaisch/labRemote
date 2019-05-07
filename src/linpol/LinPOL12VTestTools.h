#ifndef LINPOL12VTESTTOOLS_H
#define LINPOL12VTESTTOOLS_H

#include "EndeavourCom.h"
#include "EndeavourRaw.h"
#include "EndeavourRawFTDI.h"

#include "Bk85xx.h"
#include "GenericPs.h"
#include "GenericLoad.h"
#include "Keithley24XX.h"

#include <chrono>
#include <thread>
#include <json.hpp>
#include <iomanip>
#include <string>

#include "Logger.h"

using nlohmann::json;

namespace LinPOL12VTestTools
{
  // Tests linpol with 1 Load (1V4 connected) and the PBv3 comm patch adc to readout 1V4 and 
  // Voltage divided 3V3
  json testVOut_1L(GenericPs *ps, Bk85xx *load, EndeavourRawFTDI *FTDI_dev, int curr_low, int curr_hi, int curr_step = 10, int n_vals = 100);

  // Tests linpol with 2 DC loads 
  json testVOut_2L(GenericPs *ps, Bk85xx *load, GenericLoad *load2, int curr_low1, int curr_hi1,int curr_low2, int curr_hi2 , int curr_step1 = 10, int curr_step2 = 5, int n_vals = 100);
  std::string getTimeAsString(std::chrono::system_clock::time_point t);
}

#endif
