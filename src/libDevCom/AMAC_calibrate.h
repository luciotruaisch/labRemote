#ifndef AMAC_CALIBRATE_H
#define AMAC_CALIBRATE_H

#include <csdint>
#include <string>

#include "AMAC.h"

//The class is used to store information about AMAC calibration
class AMAC_calibrate {
 public:
  AMAC_calibrate(string ID, Channel CH, uint8_t BandgapControl, uint8_t RampGain);
  ~AMAC_calibrate();

  enum Channel
  {CH0_L,CH0_R,CH1_R,CH2_R,CH3_LGain1,CH3_L_Gain23,CH3_R_Gain1,CH3_R_Gain23};
  
  //Change parameter for calibrate
  void setParameter(Channel CH, uint8_t BandgapControl, uint8_t RampGain);

  //Measure of Leakage current
  double Leakage_calibrate(int count); 

 private:

  string m_ID;
  double m_m;
  double m_b;
}
#endif //AMAC_CALIBRATE_H
