#ifndef GAINCALIBRATION_H
#define GAINCALIBRATION_H

#include "DeviceCalibration.h"

//Calibrate devices with a variable Gain and 2Complements Output
class GainCalibration : public DeviceCalibration
{
 public:
  GainCalibration(double reference, uint8_t n_bit, uint8_t gain);
  virtual ~GainCalibration();

  virtual double calibrate(uint32_t counts);
  virtual uint32_t uncalibrate(double value);

 private:
  //Voltage reference
  double   m_reference;
  //Nr. of bit resolution
  uint8_t m_bit;
  //Gain of Output
  uint8_t m_gain;
};

#endif //GAINCALIBRATION_H
