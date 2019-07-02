#ifndef AMAC_CALIBRATE_H
#define AMAC_CALIBRATE_H

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

#include "AMAC.h"

//The class is used to store information about AMAC calibration
class AMAC_calibrate {
 public:

  enum Channel
    {CH0_L,CH0_R,CH1_R,CH2_R,CH3_LGain1,CH3_L_Gain23,CH3_R_Gain1,CH3_R_Gain23};

  AMAC_calibrate(std::string ID, Channel CH, uint8_t BandgapControl, uint8_t RampGain);
  ~AMAC_calibrate();
  
  //Change parameter for calibrate
  //void setParameter(Channel CH, uint8_t BandgapControl, uint8_t RampGain);

  //Measure of Leakage current
  // double Leakage_calibrate(int count); 

 private:

  std::string m_ID;
  double m_m;
  double m_b;
};

void Go2Line(std::fstream& file, unsigned int num);

#endif //AMAC_CALIBRATE_H
