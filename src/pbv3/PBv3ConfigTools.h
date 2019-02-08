#ifndef PBV3CONFIGTOOLS_H
#define PBV3CONFIGTOOLS_H

#include "AMACv2.h"

#include "Bk85xx.h"
#include "GenericPs.h"
#include "Keithley24XX.h"

#include <chrono>
#include <thread>
#include <json.hpp>
#include <iomanip>
#include <string>
#include <memory>

#include "Logger.h"

using nlohmann::json;

/**
 * \brief Module for configuring the AMACv2 using JSON files.
 *
 * The module can be split into three functions
 *  - Applying the configuration of in a JSON file to an AMAC object (config)
 *  - Finding the optimal settings for an AMAC (tune)
 *  - Determining the AMAC response to input using fixed settings (calibrate)
 *
 * All tune/calibrate functions modify the state of the AMAC object based on the result.
 *
 * All functions return a consistent configuration for the chip. For example,
 * tunning the AM bandgap changes the ADC response. Thus the result of the tuneAMBG
 * also contains updated calibration.
 */
namespace PBv3ConfigTools
{
  //
  // Copy JSON configuration to AMAC

  /**
   * \brief Configure an AMAC using the contents of a JSON file.
   *
   * Configuration of registers means changing the default values.
   *
   * \param amac Pointer to the AMACv2 object
   * \param config JSON configuration to apply
   * \param write Write the results to the AMACv2 chip in addition to updating the default values
   */
  void configAMAC(std::shared_ptr<AMACv2> amac, const json& config, bool write);

  //
  // All helper functions for tuning the AMAC settings

  /**
   * \brief Find the VDDbg setting for VDDREG to be 1.2V
   *
   * Assumes that the AMACv2 is correctly calibrated.
   *
   * Modifes the following registers:
   *  - VDDbg
   *  - VDDbgen
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json configuration
   */
  json tuneVDDBG   (std::shared_ptr<AMACv2> amac);

  /**
   * \brief Find the AMbg setting for AM bandgap to be 600 mV
   *
   * Modifes the following registers:
   *  - AMbg
   *  - AMbgen
   *
   * Updates the ADC calibration.
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json configuration and calibration
   */
  json tuneAMBG    (std::shared_ptr<AMACv2> amac);

  /**
   * \brief Find the ramp gain for ADC response of 1mV / count
   *
   * Modifes the following registers:
   *  - AMintCalib
   *
   * Updates the ADC calibration.
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json configuration and calibration
   */
  json tuneRampGain(std::shared_ptr<AMACv2> amac);

  /**
   * \brief Find the ramp gain for ADC response of 1mV / count
   *
   * Modifes the following registers:
   *  - AMintCalib
   *
   * Updates the ADC calibration.
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json configuration and calibration
   */
  json tuneCur10V(std::shared_ptr<AMACv2> amac);

  /**
   * \brief Find the ramp gain for ADC response of 1mV / count
   *
   * Modifes the following registers:
   *  - AMintCalib
   *
   * Updates the ADC calibration.
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json configuration and calibration
   */
  json tuneCur1V(std::shared_ptr<AMACv2> amac);

  //
  // All helper functions for calibration of the AMAC

  /**
   * \brief Calibrate the ADC response slope
   *
   * Updates the ADC calibration slope in mV/count.
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json calibration
   */
  json calibrateSlope (std::shared_ptr<AMACv2> amac);

  /**
   * \brief Calibrate the ADC response offsets
   *
   * Updates the ADC calibration offsets in counts.
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json calibration
   */
  json calibrateOffset(std::shared_ptr<AMACv2> amac);

  /**
   * \brief Calibrate the NTC reference voltage
   *
   * Updates the NTC reference voltages (pb,x,y) in mV.
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json calibration
   */
  json calibrateNTC(std::shared_ptr<AMACv2> amac);
}

#endif
