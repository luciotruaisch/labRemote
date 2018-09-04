#include "PBv2TB.h"

#include "I2CCom.h"
#include "I2CDevCom.h"
#include "PCA9548ACom.h"

#include "AD799X.h"

#include "LinearCalibration.h"

PBv2TB::PBv2TB(const std::string& i2cdev)
{
  // Create the I2C mappings
  std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

  m_adc_pwr=std::make_shared<AD799X>(3.3, AD799X::AD7993, std::make_shared<PCA9548ACom>(0x22, 2, mux0_com));
  m_adc_lv0=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x21, 1, mux0_com));
  m_adc_lv1=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x22, 1, mux0_com));

  //
  // Load calibrations for the different channels
  m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN     , std::make_shared<LinearCalibration>(3.3*(35+90.9)/35,0xFF));
  m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN_CURR, std::make_shared<LinearCalibration>(3.3/(50*0.005)  ,0xFF));
  m_adc_pwr->setCalibration(PBV2_ADC_CH_P5V_CURR, std::make_shared<LinearCalibration>(3.3/(50*0.1)    ,0xFF));
  m_adc_pwr->setCalibration(PBV2_ADC_CH_M5V_CURR, std::make_shared<LinearCalibration>(3.3/(50*0.1)    ,0xFF));
}

PBv2TB::~PBv2TB()
{ }

double PBv2TB::getVin()
{
  return 0.; //m_adc_pwr->read(PBV2_ADC_CH_VIN)*(35+90.9)/35;
}

double PBv2TB::getVinCurrent()
{
  return 0.; //m_adc_pwr->read(PBV2_ADC_CH_VIN_CURR)/(50*0.005);
}

double PBv2TB::getP5VCurrent()
{
  return m_adc_pwr->read(PBV2_ADC_CH_P5V_CURR);
}

double PBv2TB::getM5VCurrent()
{
  return m_adc_pwr->read(PBV2_ADC_CH_M5V_CURR);
}
