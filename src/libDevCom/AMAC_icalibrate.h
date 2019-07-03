#ifndef AMAC_ICALIBRATE_H
#define AMAC_ICALIBRATE_H

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

#include "AMAC.h"

//The class is used to store information about AMAC calibration
class AMAC_icalibrate {
 public:

  enum Channel
  {LEFT,RIGHT};

  AMAC_icalibrate(std::string ID, Channel CH, uint8_t BandgapControl, uint8_t RampGain, uint8_t OpAmpGain);
  AMAC_icalibrate(std::string ID);
  ~AMAC_icalibrate();
  
  //Change parameter for calibrate
  void setParameter(Channel CH, uint8_t BandgapControl, uint8_t RampGain, uint8_t OpAmpGain);

  //Measure of Leakage current
  double Leakage_calibrate(unsigned count); 

 private:

  std::string m_ID;
  double m_m;
  double m_b;
  double m_Voff;
  double m_RI;
};

void Go2Line(std::fstream& file, unsigned int num);

#endif //AMAC_CALIBRATE_H
