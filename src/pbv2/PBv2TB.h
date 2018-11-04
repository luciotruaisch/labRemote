#ifndef PBV2TB_H
#define PBV2TB_H

#include <memory>

#include "I2CCom.h"
#include "ADCDevice.h"
#include "DACDevice.h"
#include "AMAC.h"

#define PBV2_ADC_CH_VIN 2
#define PBV2_ADC_CH_VIN_CURR 0
#define PBV2_ADC_CH_P5V_CURR 3
#define PBV2_ADC_CH_M5V_CURR 1

#define DAC_CH_LOAD_PB1 1
#define DAC_CH_LOAD_PB2 2
#define DAC_CH_LOAD_PB3 0
#define DAC_CH_LOAD_PB4 3
#define DAC_CH_LOAD_PB5 1
#define DAC_CH_LOAD_PB6 2 
#define DAC_CH_LOAD_PB7 0
#define DAC_CH_LOAD_PB8 3
#define DAC_CH_LOAD_PB9 1

#define ADC_VOUT_PB1 6
#define ADC_VOUT_PB2 7
#define ADC_VOUT_PB3 5
#define ADC_VOUT_PB4 3
#define ADC_VOUT_PB5 1
#define ADC_VOUT_PB6 0
#define ADC_VOUT_PB7 2
#define ADC_VOUT_PB8 4
#define ADC_VOUT_PB9 6

#define ADC_HVOUT_PB1 2
#define ADC_HVOUT_PB2 3
#define ADC_HVOUT_PB3 0
#define ADC_HVOUT_PB4 1
#define ADC_HVOUT_PB5 2
#define ADC_HVOUT_PB6 3
#define ADC_HVOUT_PB7 0
#define ADC_HVOUT_PB8 1
#define ADC_HVOUT_PB9 2

#define NTC_CH_L1L3 4
#define NTC_CH_L3L5 2
#define NTC_CH_L5L7 0
#define NTC_CH_L7L9 1
#define NTC_CH_L2L4 3
#define NTC_CH_L4L6 5
#define NTC_CH_L6L8 7

class PBv2TB
{
public:
  PBv2TB(const std::string& i2cdev="/dev/i2c-0");
  ~PBv2TB();

  std::shared_ptr<AMAC> getPB(uint8_t pb);
  
  double getVin();
  double getVinCurrent();
  double getP5VCurrent();
  double getM5VCurrent();

  double getNTC(uint8_t ntc);

  double setLoad      (uint8_t pbNum, double   load);
  void   setLoadCounts(uint8_t pbNum, uint32_t counts);

  double getVout(uint8_t pbNum);
  double getHVout(uint8_t pbNum);
  
private:
  //
  // I2C multiplexers
  std::shared_ptr<I2CCom> m_mux0;
  std::shared_ptr<I2CCom> m_mux1;

  std::shared_ptr<ADCDevice> m_adc_pwr;
  std::shared_ptr<ADCDevice> m_adc_lv0;
  std::shared_ptr<ADCDevice> m_adc_lv1;
  std::shared_ptr<ADCDevice> m_adc_hv0;
  std::shared_ptr<ADCDevice> m_adc_hv1;
  std::shared_ptr<ADCDevice> m_adc_hv2;

  std::shared_ptr<DACDevice> m_dac_0;
  std::shared_ptr<DACDevice> m_dac_1;
  std::shared_ptr<DACDevice> m_dac_2;

  std::vector<std::shared_ptr<AMAC>> m_pbs{9};
};

#endif // PBV2TB_H
