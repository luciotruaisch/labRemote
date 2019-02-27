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
 *
 * The output format is compatible with the CONFIG test type in the ITk PD.
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

  /**
   * \brief Read current AMAC configuration and save it to a JSON object
   *
   * \param amac Pointer to the AMACv2 object
   * \param config JSON configuration to update
   */
  void saveConfigAMAC(std::shared_ptr<AMACv2> amac, json& config);

  /**
   * \brief Decorate a JSON config object with information that can be saved to a database
   *
   * Adds the following fields:
   *  - testType: "CONFIG"
   *  - institution: "LBL"
   *  - runNumber: "0-0" by default, increment lower number by 1 if already exists
   *
   * \param config JSON configuration to update
   */
  void decorateConfig(json& config);

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
   * \brief Tune the offset for the input current monitor block
   *
   * Scans through all settings for the DCDCiOffset register until it finds
   * a configuration where the ADC reads ~100 mV when the two measurements
   * shorted together.
   *
   * Also prints values for the current monitor test point values
   * when the zero calibration is enabled, but does not do anything with them.
   *
   * Modifes the following registers:
   *  - DCDCiOffset
   *
   * Updates the current offset calibration based on 100 readings.
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json configuration and calibration
   */
  json tuneCur10V(std::shared_ptr<AMACv2> amac);

  /**
   * \brief Tune the offset for the output current monitor block
   *
   * Scans through all settings for the DCDCoOffset register until it finds
   * a configuration where the ADC reads ~100 mV when the two measurements
   * shorted together.
   *
   * Also prints values for the current monitor test point values
   * when the zero calibration is disabled, but does not do anything with them.
   *
   * Modifes the following registers:
   *  - DCDCoOffset
   *
   * Updates the current offset calibration based on 100 readings.
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

  /**
   * \brief Calibrate the input Current Monitor offset
   *
   * Updates the offset calibration for input CM
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json calibration
   */
  json calibrateCur10V(std::shared_ptr<AMACv2> amac);

  /**
   * \brief Calibrate the output Current Monitor offset
   *
   * Updates the offset calibration for output CM
   *
   * \param amac Pointer to the AMACv2 object
   *
   * \return updated json calibration
   */
  json calibrateCur1V(std::shared_ptr<AMACv2> amac);

/**
   *Calls all brief calibration functions simultaneously,
   *
   * Writes outputs to single json object
   **/

  json calibrateAll(std::shared_ptr<AMACv2> amac);
 
}

#endif
