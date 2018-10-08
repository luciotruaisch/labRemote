#include "PBv2TB.h"

#include "I2CCom.h"
#include "I2CDevCom.h"
#include "PCA9548ACom.h"

#include "AD799X.h"
#include "DAC5574.h"

#include "LinearCalibration.h"
#include "DeviceCalibration.h"
#include "AdjustedCalibration.h"

PBv2TB::PBv2TB(const std::string& i2cdev)
{
  // Create the I2C mappings
  std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

  m_adc_pwr=std::make_shared<AD799X>(3.3, AD799X::AD7993, std::make_shared<PCA9548ACom>(0x22, 2, mux0_com));
  m_adc_lv0=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x21, 1, mux0_com));
  m_adc_lv1=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x22, 1, mux0_com));

  m_dac_0=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4C, 1, mux0_com));
  m_dac_1=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4D, 1, mux0_com));
  m_dac_2=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4E, 1, mux0_com));

  //
  // Load calibrations for the different channels
  
  m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN     , std::make_shared<LinearCalibration>(3.3*(35+90.9)/35,0x3FF));
  //m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN     , std::make_shared<AdjustedCalibration>(3.3*(35+90.9)/35,0x3FF, 0.));
  //m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN     , std::make_shared<DummyCalibration>());
  m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN_CURR, std::make_shared<LinearCalibration>(3.3/(50*0.005)  ,0x3FF));
  //m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN_CURR, std::make_shared<AdjustedCalibration>(3.3/(50*0.005)  ,0x3FF, 4.4));
  //m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN_CURR, std::make_shared<DummyCalibration>());
  m_adc_pwr->setCalibration(PBV2_ADC_CH_P5V_CURR, std::make_shared<LinearCalibration>(3.3/(50*0.1)    ,0x3FF));
  m_adc_pwr->setCalibration(PBV2_ADC_CH_M5V_CURR, std::make_shared<LinearCalibration>(3.3/(50*0.1)    ,0x3FF));

  
}

PBv2TB::~PBv2TB()
{ }

double PBv2TB::getVin()
{
  return m_adc_pwr->read(PBV2_ADC_CH_VIN);
}

double PBv2TB::getVinCurrent()
{
  return m_adc_pwr->read(PBV2_ADC_CH_VIN_CURR);
}

double PBv2TB::getP5VCurrent()
{
  return m_adc_pwr->read(PBV2_ADC_CH_P5V_CURR);
}

double PBv2TB::getM5VCurrent()
{
  return m_adc_pwr->read(PBV2_ADC_CH_M5V_CURR);
}

double PBv2TB::setDac(int DACNum, int CHVALUE, double val)
{
  if(DACNum == 0)
  {
    m_dac_0->set(CHVALUE, val);
    return 1;
  }

  else if(DACNum == 1)
  {
    m_dac_1->set(CHVALUE, val);
    return 1;
  }

  else if(DACNum == 2)
  {  
    m_dac_2->set(CHVALUE, val);
    return 1;
  }

  else
    return -1;

}

double PBv2TB::readDac(int DACNum, int CHVALUE)
{
  if(DACNum == 0)
    return m_dac_0->read(CHVALUE);

  else if(DACNum == 1)
    return m_dac_1->read(CHVALUE);

  else if(DACNum == 2)  
    return m_dac_2->read(CHVALUE); 

  else
    return -1;
}

int PBv2TB::getDAC_CH(int pbNum)
{
  if(pbNum == 1)
    return DAC_CH_LOAD_PB1;

  else if(pbNum == 2)
    return DAC_CH_LOAD_PB2;
  
  else if(pbNum == 3)
    return DAC_CH_LOAD_PB3;

  else if(pbNum == 4)
    return DAC_CH_LOAD_PB4;

  else if(pbNum == 5)
    return DAC_CH_LOAD_PB5;

  else if(pbNum == 6)
    return DAC_CH_LOAD_PB6;

  else if(pbNum == 7)
    return DAC_CH_LOAD_PB7;

  else if(pbNum == 8)
    return DAC_CH_LOAD_PB8;

  else if(pbNum == 9)
    return DAC_CH_LOAD_PB9;

  else
    return -1;
}

int PBv2TB::getDAC_Num(int pbNum)
{
  if(pbNum == 1 || pbNum == 2 || pbNum == 3 || pbNum == 4)
    return 0;

  else if(pbNum == 5 || pbNum == 6 || pbNum == 7 || pbNum == 8)
    return 1;
  
  else if(pbNum == 9)
    return 2;

  else 
    return -1;
}

double PBv2TB::getVout(int pbNum){
  switch ( pbNum ) {

  case 1 : 
    return m_adc_lv1->read(6);
    break;

  case 2 : 
    return m_adc_lv0->read(7);
    break;

  case 3 : 
    return m_adc_lv0->read(5);
    break;

  case 4 : 
    return m_adc_lv0->read(3);
    break;

  case 5 : 
    return m_adc_lv0->read(1);
    break;

  case 6 : 
    return m_adc_lv0->read(0);
    break;

  case 7 : 
    return m_adc_lv0->read(2);
    break;

  case 8 : 
    return m_adc_lv0->read(4);
    break;

  default : 
    return m_adc_lv0->read(6);
  }
}

/*
std::string getTestName(std::string TestName, int i){
  switch (i)
  {
    case 1 : 
      return TestName + pb1;
      break;
  }
}
*/