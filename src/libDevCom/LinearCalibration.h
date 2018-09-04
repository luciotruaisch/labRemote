#ifndef LINEARCALIBRATION_H
#define LINEARCALIBRATION_H

#include "DeviceCalibration.h"

class LinearCalibration : public DeviceCalibration
{
public:
  LinearCalibration(double reference, uint32_t max);
  virtual ~LinearCalibration();

  virtual double calibrate(uint32_t counts);
  virtual uint32_t uncalibrate(double value);

private:
  double m_reference;
  uint32_t m_max;
};

#endif // LINEARCALIBRATION_H
