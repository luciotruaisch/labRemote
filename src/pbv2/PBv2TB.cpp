#include "PBv2TB.h"

#include "I2CCom.h"
#include "I2CDevCom.h"
#include "PCA9548ACom.h"

#include "AD799X.h"
#include "DAC5574.h"

#include "LinearCalibration.h"
#include "DeviceCalibration.h"
#include "FileCalibration.h"

PBv2TB::PBv2TB(const std::string& i2cdev)
{
  //
  // Create the I2C mappings

  // Multiplexers
  m_mux0=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");
  m_mux1=std::make_shared<I2CDevCom>(0x74, "/dev/i2c-0");

  // ADCs
  m_adc_pwr=std::make_shared<AD799X>(3.3, AD799X::AD7993, std::make_shared<PCA9548ACom>(0x22, 2, m_mux0));
  m_adc_lv0=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x21, 1, m_mux0));
  m_adc_lv1=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x22, 1, m_mux0));

  // DACs
  double maxcurr=3.3*4.02/(100+4.02)/25e-3;
  m_dac_0=std::make_shared<DAC5574>(maxcurr, std::make_shared<PCA9548ACom>(0x4C, 1, m_mux0));
  m_dac_1=std::make_shared<DAC5574>(maxcurr, std::make_shared<PCA9548ACom>(0x4D, 1, m_mux0));
  m_dac_2=std::make_shared<DAC5574>(maxcurr, std::make_shared<PCA9548ACom>(0x4E, 1, m_mux0));

  m_dac_0->setCalibration(DAC_CH_LOAD_PB1, std::make_shared<FileCalibration>("load1.csv"));
  m_dac_0->setCalibration(DAC_CH_LOAD_PB2, std::make_shared<FileCalibration>("load2.csv"));
  m_dac_0->setCalibration(DAC_CH_LOAD_PB3, std::make_shared<FileCalibration>("load3.csv"));
  m_dac_0->setCalibration(DAC_CH_LOAD_PB4, std::make_shared<FileCalibration>("load4.csv"));
  m_dac_1->setCalibration(DAC_CH_LOAD_PB5, std::make_shared<FileCalibration>("load5.csv"));
  m_dac_1->setCalibration(DAC_CH_LOAD_PB6, std::make_shared<FileCalibration>("load6.csv"));
  m_dac_1->setCalibration(DAC_CH_LOAD_PB7, std::make_shared<FileCalibration>("load7.csv"));
  m_dac_1->setCalibration(DAC_CH_LOAD_PB8, std::make_shared<FileCalibration>("load8.csv"));
  m_dac_2->setCalibration(DAC_CH_LOAD_PB9, std::make_shared<FileCalibration>("load9.csv"));

  // PBs
  m_pbs[0]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,7, m_mux1));
  m_pbs[1]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,6, m_mux1));
  m_pbs[2]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,5, m_mux1));
  m_pbs[3]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,4, m_mux1));
  m_pbs[4]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,3, m_mux1));
  m_pbs[5]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,2, m_mux1));
  m_pbs[6]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,1, m_mux1));
  m_pbs[7]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,0, m_mux1));
  m_pbs[8]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,7, m_mux0));

  //
  // Load calibrations for the different channels
  m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN     , std::make_shared<LinearCalibration>(3.3*(35+90.9)/35,0x3FF));
  m_adc_pwr->setCalibration(PBV2_ADC_CH_VIN_CURR, std::make_shared<LinearCalibration>(3.3/(50*0.005)  ,0x3FF));
  m_adc_pwr->setCalibration(PBV2_ADC_CH_P5V_CURR, std::make_shared<LinearCalibration>(3.3/(50*0.1)    ,0x3FF));
  m_adc_pwr->setCalibration(PBV2_ADC_CH_M5V_CURR, std::make_shared<LinearCalibration>(3.3/(50*0.1)    ,0x3FF));


  // Initialize the PBs
  for(uint8_t i=0;i<9;i++)
    {
      m_pbs[i]->init();
      setLoad(i,0);
    }
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

std::shared_ptr<AMAC> PBv2TB::getPB(uint8_t pb)
{
  return m_pbs[pb];
}

double PBv2TB::setLoad(uint8_t pbNum, double load)
{
  switch ( pbNum )
    {
    case 0 :
      return m_dac_0->set(DAC_CH_LOAD_PB1, load);
      break;

    case 1 : 
      return m_dac_0->set(DAC_CH_LOAD_PB2, load);
      break;

    case 2 : 
      return m_dac_0->set(DAC_CH_LOAD_PB3, load);
      break;

    case 3 : 
      return m_dac_0->set(DAC_CH_LOAD_PB4, load);
      break;

    case 4 : 
      return m_dac_1->set(DAC_CH_LOAD_PB5, load);
      break;

    case 5 : 
      return m_dac_1->set(DAC_CH_LOAD_PB6, load);
      break;

    case 6 :
      return m_dac_1->set(DAC_CH_LOAD_PB7, load);
      break;

    case 7 :
      return m_dac_1->set(DAC_CH_LOAD_PB8, load);
      break;

    case 8 : 
      return m_dac_2->set(DAC_CH_LOAD_PB9, load);
      break;

    default:
      // TODO throw exception
      return 0.;
    }
}

void PBv2TB::setLoadCounts(uint8_t pbNum, uint32_t counts)
{
  switch ( pbNum )
    {
    case 0 :
      m_dac_0->setCount(DAC_CH_LOAD_PB1, counts);
      break;

    case 1 : 
      m_dac_0->setCount(DAC_CH_LOAD_PB2, counts);
      break;

    case 2 : 
      m_dac_0->setCount(DAC_CH_LOAD_PB3, counts);
      break;

    case 3 : 
      m_dac_0->setCount(DAC_CH_LOAD_PB4, counts);
      break;

    case 4 : 
      m_dac_1->setCount(DAC_CH_LOAD_PB5, counts);
      break;

    case 5 : 
      m_dac_1->setCount(DAC_CH_LOAD_PB6, counts);
      break;

    case 6 :
      m_dac_1->setCount(DAC_CH_LOAD_PB7, counts);
      break;

    case 7 :
      m_dac_1->setCount(DAC_CH_LOAD_PB8, counts);
      break;

    case 8 : 
      m_dac_2->setCount(DAC_CH_LOAD_PB9, counts);
      break;

    default:
      // TODO throw exception
      break;
    }
}

double PBv2TB::getVout(uint8_t pbNum)
{
  switch ( pbNum )
    {
    case 0 : 
      return m_adc_lv1->read(ADC_VOUT_PB1);
      break;

    case 1 : 
      return m_adc_lv0->read(ADC_VOUT_PB2);
      break;

    case 2 : 
      return m_adc_lv0->read(ADC_VOUT_PB3);
      break;

    case 3 : 
      return m_adc_lv0->read(ADC_VOUT_PB4);
      break;

    case 4 : 
      return m_adc_lv0->read(ADC_VOUT_PB5);
      break;

    case 5 : 
      return m_adc_lv0->read(ADC_VOUT_PB6);
      break;

    case 6 : 
      return m_adc_lv0->read(ADC_VOUT_PB7);
      break;

    case 7 : 
      return m_adc_lv0->read(ADC_VOUT_PB8);
      break;

    case 8 : 
      return m_adc_lv0->read(ADC_VOUT_PB9);
      break;

    default:
      // TODO throw exception
      return 0.;
    }
}

