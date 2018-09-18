#ifndef PBV2TB_H
#define PBV2TB_H

#include <memory>
#include <string>

#include "ADCDevice.h"

#define PBV2_ADC_CH_VIN 2
#define PBV2_ADC_CH_VIN_CURR 0
#define PBV2_ADC_CH_P5V_CURR 3
#define PBV2_ADC_CH_M5V_CURR 1

class PBv2TB
{
public:
  PBv2TB(const std::string& i2cdev="/dev/i2c-0");
  ~PBv2TB();

  double getVin();
  double getVinCurrent();
  double getP5VCurrent();
  double getM5VCurrent();
  
private:
  std::shared_ptr<ADCDevice> m_adc_pwr;
  std::shared_ptr<ADCDevice> m_adc_lv0;
  std::shared_ptr<ADCDevice> m_adc_lv1;
};

#endif // PBV2TB_H
